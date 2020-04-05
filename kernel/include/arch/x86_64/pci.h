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

void init_pci();
