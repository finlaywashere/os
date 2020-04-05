#include <arch/x86_64/pci.h>
#include <arch/x86_64/io.h>
#include <kernel/string.h>

pci_bus_t *busses;

uint16_t pciConfigReadWord(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset){
	uint32_t address;
	uint32_t lbus = (uint32_t) bus;
	uint32_t lslot = (uint32_t) device;
	uint32_t lfunc = (uint32_t) function;
	uint16_t tmp = 0;

	address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

	outd(0xCF8,address);

	tmp = (uint16_t)((ind(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);

	return tmp;
}

pci_device_function_t* getFunction(uint8_t bus, uint8_t device, uint8_t function){
	uint16_t vendorID = pciConfigReadWord(bus, device, function, 0);
	if(vendorID == 0xFFFF)
		return 0x0;
	pci_device_function_t *ret_function = kmalloc(sizeof(pci_device_function_t));
	ret_function->vendorID = vendorID;
	uint16_t deviceID = pciConfigReadWord(bus, device, function, 0x2);
	ret_function->deviceID = deviceID;
	uint16_t command = pciConfigReadWord(bus, deviceID, function, 0x4);
	ret_function->command = command;
	uint16_t status = pciConfigReadWord(bus, deviceID, function, 0x6);
	ret_function->status = status;
	uint16_t revProg = pciConfigReadWord(bus, deviceID, function, 0x8);
	uint8_t revID = (uint8_t)(revProg & 0xFF);
	ret_function->revisionID = revID;
	uint8_t progIF = (uint8_t)(revProg>>8);
	ret_function->progIF = progIF;
	uint16_t subclassClassCode = pciConfigReadWord(bus, deviceID, function, 0xA);
	uint8_t subclass = (uint8_t) (subclassClassCode & 0xFF);
	ret_function->subclass = subclass;
	uint8_t classcode = (uint8_t) (subclassClassCode >> 8);
	ret_function->classCode = classcode;
	uint16_t cacheLineSizeLatencyTimer = pciConfigReadWord(bus, device, function, 0xC);
	uint8_t cacheLineSize = (uint8_t)(cacheLineSizeLatencyTimer & 0xFF);
	ret_function->cacheLineSize = cacheLineSize;
	uint8_t latencyTimer = (uint8_t)(cacheLineSizeLatencyTimer >> 8);
	ret_function->latencyTimer = latencyTimer;
	uint16_t headerTypeBIST = pciConfigReadWord(bus, device, function, 0xE);
	uint8_t headerType = (uint8_t)(headerTypeBIST & 0xFF);
	ret_function->headerType = headerType;
	uint8_t bist = (uint8_t)(headerTypeBIST >> 8);
	ret_function->bist = bist;
	return ret_function;
}

void init_pci_bus(uint8_t bus){
	for(int i = 0; i < 10; i++){
		pci_device_function_t *function = getFunction(bus, i, 0);
		if(function == 0x0)
			continue;
		pci_device_t *device = kmalloc(sizeof(pci_device_t));
		memset(device,0,sizeof(pci_device_t));
		device->functions[0] = function;
		if(function->headerType & 0b01111111){
			uint8_t secondaryBusNumber = (uint8_t)(pciConfigReadWord(bus, i, 0, 0x18) >> 8);
			init_pci_bus(secondaryBusNumber);
		}else{
			if(function->headerType & 1<<7){
				for(int j = 1; j < 8; j++){
					pci_device_function_t *function = getFunction(bus, i, j);
					device->functions[j] = function;
				}
			}
		}
		busses[bus].devices[i] = device;
	}
}

void init_pci(){
	busses = kmalloc(sizeof(pci_bus_t)*256);
	memset(busses, 0, sizeof(pci_bus_t)*256);
	init_pci_bus(0);
	for(int i = 0; i < 10; i++){
		pci_device_t *device = busses[0].devices[i];
		if(device == 0x0 || device->functions[0] == 0x0)
			continue;
		pci_device_function_t function = *device->functions[0];
		terminal_writestring("Device id: 0x");
		terminal_writeint(function.deviceID,16);
		terminal_writestring("\n");
	}
}
