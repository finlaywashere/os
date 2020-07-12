#include <arch/x86_64/acpi.h>
#include <kernel/string.h>
#include <kernel/kernel.h>

rsdp_descriptor_t* init_acpi(){
	uint16_t *ebda = 0xffff80000000040e;
	uint64_t ebdaStart = *ebda + 0xffff800000000000;
	if((*ebda % 16) != 0){
		ebdaStart -= (*ebda % 16);
	}
	rsdp_descriptor_t* rsdp_descriptor = 0x0;
	for(uint64_t i = 0; i < 1024; i+=16){
		rsdp_descriptor_t* descriptor = (rsdp_descriptor_t*)(ebdaStart+i);
		if(memcmp("RSD PTR ",descriptor->signature,8) == 0){
			rsdp_descriptor = descriptor;
			break;
		}
	}
	for(uint64_t addr = 0xffff8000000e0000; addr < 0xffff8000000fffff; addr += 16){
		if(rsdp_descriptor != 0x0)
			break;
		rsdp_descriptor_t* descriptor = (rsdp_descriptor_t*) addr;
                if(memcmp("RSD PTR ",descriptor->signature,8) == 0){
                        rsdp_descriptor = descriptor;
			break;
                }
	}
	if(rsdp_descriptor == 0)
		panic("Failed to find RSDP table!");
	uint8_t sigTotal = 0;
	for(int i = 0; i < sizeof(rsdp_descriptor_t); i++){
		uint8_t *val = rsdp_descriptor+i;
		sigTotal += *val;
	}
	if((uint8_t)sigTotal != 0)
		panic("RSDP checksum invalid!");
	
	return rsdp_descriptor;
}
