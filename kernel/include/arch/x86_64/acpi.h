#include <stdint.h>
struct rsdp_descriptor{
	char signature[8];
	uint8_t checksum;
	char oem[6];
	uint8_t revision;
	uint32_t rsdtAddr;
}__attribute__((packed));
typedef struct rsdp_descriptor rsdp_descriptor_t;
struct rsdp_descriptor_2{
	rsdp_descriptor_t firstPart;
	uint32_t length;
	uint64_t xsdtAddr;
	uint8_t extendedChecksum;
	uint8_t reserved[3];
}__attribute__((packed));
typedef struct rsdp_descriptor_2 rsdp_descriptor_t_2;

rsdp_descriptor_t* init_acpi();
