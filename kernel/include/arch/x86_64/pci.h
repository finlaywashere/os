#include <stdint.h>

struct pci_device_function{
	uint16_t deviceID;
	uint16_t vendorID;
	uint16_t command;
	uint16_t status;
	uint8_t revisionID;
	uint8_t progIF;
	uint8_t subclass;
	uint8_t classCode;
	uint8_t cacheLineSize;
	uint8_t latencyTimer;
	uint8_t headerType;
	uint8_t bist;
	uint64_t bar0;
	uint64_t bar1;
	uint64_t bar2;
	uint64_t bar3;
	uint64_t bar4;
	uint64_t bar5;
	uint64_t cisPointer;
	uint16_t subsystemVendorID;
	uint16_t subsystemID;
	uint32_t expansionROMAddr;
	uint8_t capabilitiesPointer;
	uint8_t interruptLine;
	uint8_t interruptPin;
	uint8_t minGrant;
	uint8_t maxLatency;
};

typedef struct pci_device_function pci_device_function_t;

struct pci_device{
	pci_device_function_t *functions[8];
};
typedef struct pci_device pci_device_t;
struct pci_bus{
	pci_device_t *devices[10];
};
typedef struct pci_bus pci_bus_t;

struct pci_device_result{
	pci_device_function_t* devices;
	uint8_t count;
};
typedef struct pci_device_result pci_device_result_t;

void init_pci();
pci_device_result_t* findPCIDevicesByClass(uint8_t class);
