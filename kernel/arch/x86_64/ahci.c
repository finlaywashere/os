#include <arch/x86_64/ahci.h>
#include <arch/x86_64/pci.h>
#include <arch/x86_64/tty.h>
#include <kernel/paging.h>

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
	HBA_MEM *mem = (uint64_t) controller.bar5;
	
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
