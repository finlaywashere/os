#include <arch/x86_64/ahci.h>
#include <arch/x86_64/pci.h>
#include <arch/x86_64/tty.h>
#include <arch/x86_64/paging.h>

int check_type(HBA_PORT *port){
	uint32_t ssts = port->ssts;

	uint8_t ipm = (ssts >> 8) & 0x0F;
	uint8_t det = ssts & 0x0F;
	
	// check for correct interface and active state
	if((ipm != 0x01) || (det != 0x03))
		return 0;
	if(port->sig != 0)
		return 1;
	return 0;
}

int probe_port(HBA_MEM *mem, int portno){
	uint32_t pi = mem->pi;
	if(pi >> portno & 0x1 == 1){
		return check_type(&mem->ports[portno]);
	}
	return 0;
}

pci_device_function_t controller;

int find_cmdslot(HBA_PORT *port){
	// If not set in SACT and CI, the slot is free
	uint32_t slots = (port->sact | port->ci);
	for (int i=0; i<32; i++){
		if ((slots&1) == 0)
			return i;
		slots >>= 1;
	}
	terminal_writestring("Cannot find free command list entry\n");
	return -1;
}

int read(HBA_PORT *port, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf){
	port->is = (uint32_t) -1; // Clear interrupt bit
	int spin = 0; // Timeout counter
	int slot = find_cmdslot(port);
	if(slot == -1)
		return 1;
	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
	cmdheader+=slot;
	cmdheader->cfl = sizeof(FIS_REG_H2D)/sizeof(uint32_t);
	cmdheader->w = 0; // Read from device
	cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1; //PRDT entries count
	
	HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba);
	memset(cmdtbl,0,sizeof(HBA_CMD_TBL)+(cmdheader->prdtl-1)*sizeof(HBA_PRDT_ENTRY));
	for(int i = 0; i < cmdheader->prdtl-1; i++){ //8K bytes, 16 sectors, per PRDT
		cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
		cmdtbl->prdt_entry[i].dbc = 8*1024-1; //8K bytes, this needs to be 8K minus 1
		cmdtbl->prdt_entry[i].i = i;
		buf += 4*1024; // 4K words
		count -= 16; // 16 sectors
	}
	int i = cmdheader->prdtl-1;
	cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
	cmdtbl->prdt_entry[i].dbc = (count<<9)-1; //512 bytes per sector
	cmdtbl->prdt_entry[i].i = i;
	
	//Setup command
	FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);
	
	cmdfis->fis_type = FIS_TYPE_REG_H2D;
	cmdfis->c = 1; //Command
	cmdfis->command = ATA_CMD_READ_DMA_EX;
	
	cmdfis->lba0 = (uint8_t) startl;
	cmdfis->lba1 = (uint8_t) (startl>>8);
	cmdfis->lba2 = (uint8_t) (startl>>16);
	cmdfis->device = 1<<6; //Set LBA mode

	cmdfis->lba3 = (uint8_t) (startl>>24);
	cmdfis->lba4 = (uint8_t) starth;
	cmdfis->lba5 = (uint8_t) (starth>>8);

	cmdfis->countl = count & 0xFF;
	cmdfis->counth = (count >> 8) & 0xFF;
	
	// Wait until port isn't busy or timeout is reached
	while((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000){
		spin++;
	}
	if(spin == 1000000){
		terminal_writestring("AHCI port is hung!\n");
		return 1;
	}
	port->ci = 1<<slot; //Issue the command
	
	while(1){
		// If the command has completed
		if((port->ci & (1<<slot)) == 0)
			break;
		if(port->is & HBA_PxIS_TFES){ //Task File Error
			terminal_writestring("AHCI read disk error!\n");
			return 1;
		}
	}
	if(port->is & HBA_PxIS_TFES){
		terminal_writestring("AHCI read disk error!\n");
		return 1;
	}
	return 0;
}
HBA_MEM *mem = 0;
HBA_PORT *getPort(int index){
	return &mem->ports[index];
}

void init_ahci(){
	pci_device_result_t* ahciDevices = findPCIDevicesByClass(0x01);
	for(int i = 0; i < ahciDevices->count; i++){
		pci_device_function_t function = ahciDevices->devices[i];
		terminal_writestring("PCI AHCI controller class: 0x");
		terminal_writeint(function.classCode,2);
		terminal_writestring("\n");
		controller = function;
		break;
	}
	mapPages(controller.bar5,controller.bar5,1<<1,sizeof(HBA_MEM)+sizeof(HBA_PORT)*32);
	mem = (uint64_t) controller.bar5;
	
	terminal_writestring("Cap: 0x");
	terminal_writeint(mem->cap,16);
	terminal_writestring("\n");

	terminal_writestring("HBA version: ");
	terminal_writeint(mem->vs,10);
	terminal_writestring("\n");
	
	int activePorts[32];
	int curr = 0;
	
	for(int i = 0; i < 32; i++){
		int result = probe_port(mem,i);
		if(result == 1){
			terminal_writestring("Found SATA drive at port 0x");
			terminal_writeint(i,16);
			terminal_writestring("\n");
			
			HBA_PORT* port = &mem->ports[i];
			if(!isMapped(port->clbu<<32|port->clb))
				mapPages(port->clbu<<32|port->clb,port->clbu<<32|port->clb,1<<1,sizeof(HBA_CMD_HEADER)*32);
			if(!isMapped(port->fbu<<32|port->fb))
				mapPage(port->fbu<<32|port->fb,port->fbu<<32|port->fb,1<<1);
			// Command table offset: 40K + 8K*portno
			// Command table size = 256*32 = 8K per port
			HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(port->clb);
			for (int i=0; i<32; i++){
				cmdheader[i].prdtl = 8;	// 8 prdt entries per command table
				// 256 bytes per command table, 64+16+48+16*8
				// Command table offset: 40K + 8K*portno + cmdheader_index*256
				if(!isMapped(cmdheader[i].ctbau<<32|cmdheader[i].ctba))
					mapPage(cmdheader[i].ctbau<<32|cmdheader[i].ctba,cmdheader[i].ctbau<<32|cmdheader[i].ctba,1<<1);
			}
			
			activePorts[curr] = i;	
			curr++;
		}
	}
	
}
