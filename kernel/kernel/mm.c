#include "kernel/pmm.h"

#include "multiboot.h"
#include "utils.h"

void initPMM(multiboot_info_t* mbd){
	multiboot_memory_map_t* entry = mbd->mmap_addr;
	while(entry < mbd->mmap_addr + mbd->mmap_length){
		uint32_t type = entry->type;
		if(type == MULTIBOOT_MEMORY_AVAILABLE){
			uint32_t addr_low = entry->addr_low;
			uint32_t addr_high = entry->addr_high;
			uint32_t len_low = entry->len_low;
			uint32_t len_high = entry->len_high;
		}
		entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
	}
}
