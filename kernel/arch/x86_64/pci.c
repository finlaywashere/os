#include <arch/x86_64/pci.h>
#include <arch/x86_64/io.h>
#include <kernel/string.h>
#include <kernel/vmm.h>

pci_bus_t busses[256];

uint16_t pciConfigReadWord(uint8_t bus, uint8_t device, uint8_t function,
		uint8_t offset) {
	uint32_t address;
	uint32_t lbus = (uint32_t) bus;
	uint32_t lslot = (uint32_t) device;
	uint32_t lfunc = (uint32_t) function;
	uint16_t tmp = 0;

	address = (uint32_t) ((lbus << 16) | (lslot << 11) | (lfunc << 8)
			| (offset & 0xFC) | ((uint32_t) 0x80000000));

	outd(0xCF8, address);

	tmp = (uint16_t) ((ind(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);

	return tmp;
}

pci_device_function_t* getFunction(uint8_t bus, uint8_t device,
		uint8_t function) {
	uint16_t vendorID = pciConfigReadWord(bus, device, function, 0);
	if (vendorID == 0xFFFF)
		return 0x0;
	pci_device_function_t *ret_function = (pci_device_function_t*) kmalloc_p(
			sizeof(pci_device_function_t));
	ret_function->vendorID = vendorID;
	uint16_t deviceID = pciConfigReadWord(bus, device, function, 0x2);
	ret_function->deviceID = deviceID;
	uint16_t command = pciConfigReadWord(bus, device, function, 0x4);
	ret_function->command = command;
	uint16_t status = pciConfigReadWord(bus, device, function, 0x6);
	ret_function->status = status;
	uint16_t revProg = pciConfigReadWord(bus, device, function, 0x8);
	uint8_t revID = (uint8_t) (revProg & 0xFF);
	ret_function->revisionID = revID;
	uint8_t progIF = (uint8_t) (revProg >> 8);
	ret_function->progIF = progIF;
	uint16_t subclassClassCode = pciConfigReadWord(bus, device, function, 0xA);
	uint8_t subclass = (uint8_t) (subclassClassCode & 0xFF);
	ret_function->subclass = subclass;
	uint8_t classcode = (uint8_t) (subclassClassCode >> 8);
	ret_function->classCode = classcode;
	uint16_t cacheLineSizeLatencyTimer = pciConfigReadWord(bus, device,
			function, 0xC);
	uint8_t cacheLineSize = (uint8_t) (cacheLineSizeLatencyTimer & 0xFF);
	ret_function->cacheLineSize = cacheLineSize;
	uint8_t latencyTimer = (uint8_t) (cacheLineSizeLatencyTimer >> 8);
	ret_function->latencyTimer = latencyTimer;
	uint16_t headerTypeBIST = pciConfigReadWord(bus, device, function, 0xE);
	uint8_t headerType = (uint8_t) (headerTypeBIST & 0xFF);
	ret_function->headerType = headerType;
	uint8_t bist = (uint8_t) (headerTypeBIST >> 8);
	ret_function->bist = bist;
	
	uint16_t bar0_0 = pciConfigReadWord(bus,device,function,0x10);
	uint16_t bar0_1 = pciConfigReadWord(bus,device,function,0x12);
	uint32_t bar0 = (uint32_t) (bar0_1 << 16) | bar0_0;
	ret_function->bar0 = bar0;
	uint16_t bar1_0 = pciConfigReadWord(bus,device,function,0x14);
	uint16_t bar1_1 = pciConfigReadWord(bus,device,function,0x16);
	uint32_t bar1 = (uint32_t) (bar1_1 << 16) | bar1_0;
	ret_function->bar1 = bar1;
	uint16_t bar2_0 = pciConfigReadWord(bus,device,function,0x18);
	uint16_t bar2_1 = pciConfigReadWord(bus,device,function,0x1A);
	uint32_t bar2 = (uint32_t) (bar2_1 << 16) | bar2_0;
	ret_function->bar2 = bar2;
	uint16_t bar3_0 = pciConfigReadWord(bus,device,function,0x1C);
	uint16_t bar3_1 = pciConfigReadWord(bus,device,function,0x1E);
	uint32_t bar3 = (uint32_t) (bar3_1 << 16) | bar3_0;
	ret_function->bar3 = bar3;
	uint16_t bar4_0 = pciConfigReadWord(bus,device,function,0x20);
	uint16_t bar4_1 = pciConfigReadWord(bus,device,function,0x22);
	uint32_t bar4 = (uint32_t) (bar4_1 << 16) | bar4_0;
	ret_function->bar4 = bar4;
	uint16_t bar5_0 = pciConfigReadWord(bus,device,function,0x24);
	uint16_t bar5_1 = pciConfigReadWord(bus,device,function,0x26);
	uint32_t bar5 = (uint32_t) (bar5_1 << 16) | bar5_0;
	ret_function->bar5 = bar5;
	
	uint16_t cis_0 = pciConfigReadWord(bus,device,function,0x28);
	uint16_t cis_1 = pciConfigReadWord(bus,device,function,0x2A);
	uint32_t cis = (uint32_t) (cis_1 << 16) | cis_0;
	ret_function->cisPointer = cis;
	
	uint16_t subsystemVendor = pciConfigReadWord(bus,device,function,0x2C);
	ret_function->subsystemVendorID = subsystemVendor;
	
	uint16_t subsystemID = pciConfigReadWord(bus,device,function,0x2E);
	ret_function->subsystemID = subsystemID;
	
	uint16_t expansionAddr_0 = pciConfigReadWord(bus,device,function,0x30);
	uint16_t expansionAddr_1 = pciConfigReadWord(bus,device,function,0x32);
	uint32_t expansionAddr = (uint32_t) (expansionAddr_1 << 16) | expansionAddr_0;
	ret_function->expansionROMAddr = expansionAddr;
	
	uint8_t capabilitiesPointer = (uint8_t) pciConfigReadWord(bus,device,function,0x34) & 0xFF;
	ret_function->capabilitiesPointer = capabilitiesPointer;
	
	uint16_t interrupt = pciConfigReadWord(bus,device,function,0x3C);
	uint8_t interruptLine = (uint8_t) interrupt & 0xFF;
	ret_function->interruptLine = interruptLine;
	uint8_t interruptPin = (uint8_t) interrupt>>8;
	ret_function->interruptPin = interruptPin;
	
	uint16_t minGrantMaxLatency = pciConfigReadWord(bus,device,function,0x3E);
	uint8_t minGrant = (uint8_t) minGrantMaxLatency & 0xFF;
	ret_function->minGrant = minGrant;
	uint8_t maxLatency = (uint8_t) minGrantMaxLatency >> 8;
	ret_function->maxLatency = maxLatency;
	
	return ret_function;
}

void init_pci_bus(uint8_t bus) {
	for (int i = 0; i < 10; i++) {
		pci_device_function_t *function = getFunction(bus, i, 0);
		if (function == 0x0)
			continue;
		if (function->headerType & 0b01111111) {
			terminal_writestring("Found PCI bridge!\n");
			uint8_t secondaryBusNumber = (uint8_t) (pciConfigReadWord(bus, i, 0,
					0x18) >> 8);
			init_pci_bus(secondaryBusNumber);
		} else {
			pci_device_t *device = kmalloc_p(sizeof(pci_device_t));
			memset(device, 0, sizeof(pci_device_t));
			device->functions[0] = function;
			if (function->headerType & 1 << 7) {
				for (int j = 1; j < 8; j++) {
					pci_device_function_t *function2 = getFunction(bus, i, j);
					if (function2->headerType & 0b01111111) {
						terminal_writestring("Found PCI bridge!\n");
						uint8_t secondaryBusNumber =
								(uint8_t) (pciConfigReadWord(bus, i, 0, 0x18)
										>> 8);
						init_pci_bus(secondaryBusNumber);
					}else{
						device->functions[j] = function2;
					}
				}
			}
			busses[bus].devices[i] = device;
		}
	}
}

uint8_t countPCIDevicesByClass(uint8_t class) {
	uint8_t count = 0;
	for (int i = 0; i < 256; i++) {
		pci_bus_t bus = busses[i];
		if (&bus == 0x0)
			continue;
		for (int j = 0; j < 10; j++) {
			pci_device_t *device = bus.devices[j];
			if (device == 0x0)
				continue;
			for (int j2 = 0; j2 < 8; j2++) {
				pci_device_function_t *function = device->functions[j2];
				if (function == 0x0)
					break;
				if (function->classCode == class)
					count++;
			}
		}
	}
	return count;
}
pci_device_result_t* findPCIDevicesByClass(uint8_t class) {
	uint8_t count = countPCIDevicesByClass(class);
	pci_device_result_t *result = kmalloc_p(
			sizeof(uint8_t) + sizeof(pci_device_function_t*));
	pci_device_function_t *functions = kmalloc_p(
			sizeof(pci_device_function_t) * count);
	result->count = count;
	uint8_t i2 = 0;
	for (int i = 0; i < 256; i++) {
		pci_bus_t bus = busses[i];
		if (&bus == 0x0)
			continue;
		for (int j = 0; j < 10; j++) {
			pci_device_t *device = bus.devices[j];
			if (device == 0x0)
				continue;
			for (int j2 = 0; j2 < 8; j2++) {
				pci_device_function_t *function = device->functions[j2];
				if (function == 0x0)
					break;
				if (function->classCode == class)
					functions[i2++] = *function;
			}
		}
	}
	result->devices = functions;
	return result;
}

void init_pci() {
	//busses = kmalloc_p(sizeof(pci_bus_t) * 256);
	//memset(busses, 0, sizeof(pci_bus_t) * 256);
	init_pci_bus(0);
	for (int i = 0; i < 10; i++) {
		pci_device_t *device = busses[0].devices[i];
		if (device == 0x0 || device->functions[0] == 0x0)
			continue;
		pci_device_function_t function = *device->functions[0];
		//terminal_writestring("Device class: 0x");
		//terminal_writeint(function.classCode, 16);
		//terminal_writestring("\n");
	}
}
