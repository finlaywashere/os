#include <arch/x86_64/acpi.h>
#include <kernel/string.h>
#include <kernel/kernel.h>

void validate(sdt_header_t* sdt){
	uint8_t sum = 0;
	for(int i = 0; i < sdt->length; i++){
		sum += ((uint8_t*)sdt)[i];
	}
	if(sum != 0)
		panic("SDT header checksum is invalid!");
	return;
}

sdt_header_t* find_acpi_header(rsdp_descriptor_t* rsdp, char sig[4]){
	uint64_t base_addr = rsdp->rsdtAddr+0xffff800000000000;
	sdt_header_t* rsdt = (sdt_header_t*) base_addr;
	int entries = (rsdt->length-sizeof(rsdt)) / 4;
	for(int i = 0; i < entries; i++){
		sdt_header_t* sdt = (uint64_t)*(uint32_t*)(rsdt+4*i);
		if(memcmp(sdt->signature,sig,4) == 0){
			validate(sdt);
			return sdt;
		}
	}
	return 0;
}

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
	uint8_t* rsdp_array = (uint8_t*)rsdp_descriptor;
	uint32_t sigTotal = 0;
	for(int i = 0; i < 20; i++){
		uint8_t val = rsdp_array[i];
		sigTotal += val;
	}
	if((uint8_t)sigTotal != 0)
		panic("RSDP checksum invalid!");
	
	return rsdp_descriptor;
}
