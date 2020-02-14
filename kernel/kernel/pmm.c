#include "kernel/pmm.h"

#include "multiboot.h"
#include "utils.h"

#define MEM_BLOCK_SIZE 4

extern uint64_t _kernel_end;
uint64_t kernel_end;

uint64_t* memoryMap;

void initPMM(multiboot_info_t* mbd){
	kernel_end = _kernel_end;
	uint32_t mem_size = mbd->mem_upper;
	uint32_t blocks = mem_size / MEM_BLOCK_SIZE;
	memoryMap = (char *)kernel_end;
	
	for(uint32_t i = 0; i < blocks; i++){
		memoryMap[i] = 0;
	}
	
	kernel_end += blocks * sizeof(uint64_t);
	
	multiboot_memory_map_t* entry = mbd->mmap_addr;
	uint32_t workingIndex = 0;
	while(entry < mbd->mmap_addr + mbd->mmap_length){
		uint32_t type = entry->type;
		if(type == MULTIBOOT_MEMORY_AVAILABLE){
			uint32_t addr_low = entry->addr_low;
			uint32_t addr_high = entry->addr_high;
			uint32_t len_low = entry->len_low;
			uint32_t len_high = entry->len_high;
			
			uint64_t len = (((uint64_t) len_high) << 32) | ((uint64_t) len_low);
			uint64_t addr = (((uint64_t) addr_high) << 32) | ((uint64_t) addr_low);
			
			for(uint64_t l = 0; l < len; l+=MEM_BLOCK_SIZE){
				memoryMap[workingIndex] = addr + l*1024;
				workingIndex++;
			}
		}
		entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
	}
}
uint64_t allocPBlock(uint64_t _size){
	uint64_t size = _size/(1024*MEM_BLOCK_SIZE) + 1;
	uint32_t i = 0;
	while(true){
		uint64_t block = memoryMap[i];
		if(!(block < kernel_end)){
			uint32_t found = 1;
			for(uint32_t i2 = 0; i2 < size; i2++){
				if(memoryMap[i] < kernel_end){
					found = 0;
					break;
				}
				i++;
			}
			if(!found)
				continue;
			return block;
		}
		i++;
	}
}
void freePBlock(uint64_t addr, uint64_t _size){
	uint64_t size = ((uint64_t) _size)/(1024*MEM_BLOCK_SIZE) + 1;
        uint32_t i = 0;
        while(true){
                uint64_t block = memoryMap[i];
                if(!(block < kernel_end)){
                        uint32_t found = 1;
                        for(uint32_t i2 = 0; i2 < size; i2++){
                                if(memoryMap[i] > kernel_end){
                                        found = 0;
                                        break;
                                }
                                i++;
                        }
                        if(!found)
                                continue;
			uint32_t finalIndex = i + size;
                        for(uint32_t startIndex = i ;i < finalIndex; i++){
				memoryMap[i] = addr+(i-startIndex)*1024*MEM_BLOCK_SIZE;
			}
			return;
                }
                i++;
        }

}
