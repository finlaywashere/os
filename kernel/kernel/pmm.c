#include "kernel/pmm.h"

#include "multiboot.h"
#include "utils.h"
#include "arch/x86/tty.h"

#define MEM_BLOCK_SIZE 8192

extern uint64_t _kernel_end;
uint64_t kernel_end;

struct block {
	uint64_t addr;
	uint64_t size;
};

struct block *free_addrs;
uint64_t free_addrs_size;
uint64_t last_index = 0;

uint64_t latest_addr = 0x100000;

void* malloc(uint64_t size) {
	for (int i = 0; i < free_addrs_size; i++) {
		if (free_addrs[i].size >= size){
			uint64_t addr = free_addrs[i].addr;
			
			free_addrs[i].addr += size;
			free_addrs[i].size -= size;
			
			return (void*)addr;
		}
	}
	
	// no free blocks were found in the list, default to bump allocator
	void* ret = (void*)latest_addr;
	latest_addr += size;
	return ret;
}
void free(uint64_t addr, uint64_t _size){
	uint64_t size = (_size/MEM_BLOCK_SIZE+1)*MEM_BLOCK_SIZE;
	//TODO: do freeing stuff
}

void initPMM(multiboot_info_t* mbd){
	kernel_end = _kernel_end;
	free_addrs = kernel_end;
	uint32_t mem_size = mbd->mem_upper;
	uint32_t blocks = mem_size / MEM_BLOCK_SIZE;
	free_addrs_size = blocks;
	
	kernel_end += blocks * sizeof(struct block);
	
	multiboot_memory_map_t* entry = mbd->mmap_addr;
	int workingI = 0;
	while(entry < mbd->mmap_addr + mbd->mmap_length){
		uint32_t type = entry->type;
		if(type == MULTIBOOT_MEMORY_AVAILABLE){
			uint32_t addr_low = entry->addr_low;
			uint32_t addr_high = entry->addr_high;
			uint32_t len_low = entry->len_low;
			uint32_t len_high = entry->len_high;
			
			uint64_t len = (((uint64_t) len_high) << 32) | ((uint64_t) len_low);
			uint64_t addr = (((uint64_t) addr_high) << 32) | ((uint64_t) addr_low);
			free_addrs[workingI].addr = addr;
			free_addrs[workingI++].size = len;
		}
		entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
	}
}
