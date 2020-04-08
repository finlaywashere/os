#include <arch/x86_64/ahci.h>
#include <arch/x86_64/pci.h>
#include <arch/x86_64/tty.h>

int check_type(HBA_PORT *port){
	uint32_t ssts = port->ssts;

	uint8_t ipm = (ssts >> 8) & 0x0F;
	uint8_t det = ssts & 0x0F;
	
	// check for correct interface and active state
	if((ipm != 0x01) || (det != 0x03))
		return 0;
	if(port->sig == 1)
		return 1;
	return 0;
}

int probe_port(HBA_MEM *mem, int portno){
	uint32_t pi = mem->pi;
	if(pi & 1){
		return check_type(&mem->ports[portnum]);
	}
	return 0;
}

void start_cmd(HBA_PORT *port){
	port->cmd &= ~0x0001;
	
	while(port->cmd & 0x8000);
	
	port->cmd |= 0x0010;
	port->cmd |= 0x0001;
}
void stop_cmd(HBA_PORT *port){
	port->cmd &= ~0x0001;
	
	while(port->cmd & 0x8000);
	
	port->cmd &= ~0x0010;
}

void init_ahci(){
	pci_device_result_t* ahciDevices = findPCIDevicesByClass(0x01);
	pci_device_function_t controller;
	for(int i = 0; i < ahciDevices->count; i++){
		pci_device_function_t function = ahciDevices->devices[i];
		terminal_writestring("PCI AHCI controller class: 0x");
		terminal_writeint(function.classCode,2);
		terminal_writestring("\n");
		controller = function;
		break;
	}
	HBA_MEM *mem = controller.bar5;
	
	for(int i = 0; i < 32; i++){
		int result = probe_port(mem,i);
		if(result == 1){
			terminal_writestring("Found SATA drive at port 0x");
			terminal_writeint(i,16);
			terminal_writestring("\n");
		}
	}
	
}
