#include <arch/x86_64/ahci.h>
#include <arch/x86_64/pci.h>
#include <arch/x86_64/tty.h>

void init_ahci(){
	pci_device_result_t* ahciDevices = findPCIDevicesByClass(0x01);
	for(int i = 0; i < ahciDevices->count; i++){
		pci_device_function_t function = ahciDevices->devices[i];
		terminal_writestring("PCI AHCI controller class: 0x");
		terminal_writeint(function.classCode,2);
		terminal_writestring("\n");
	}
	FIS_REG_H2D fis;
	memset(&fis,0,sizeof(FIS_REG_H2D));
	fis.fis_type = FIS_TYPE_REG_H2D;
	fis.command = 0xEC;
	fis.device = 0;
	fis.c = 1;

}
