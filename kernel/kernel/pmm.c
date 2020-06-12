#include <kernel/pmm.h>
#include <multiboot.h>
#include <kernel/string.h>
#include <arch/x86_64/tty.h>

extern uint64_t _kernel_end;
uint64_t kernel_end;

memory_block_t *map;
uint64_t size;

void init_pmm_base(){
	kernel_end = &_kernel_end;
        kernel_end += 0x81000;
}
void init_pmm(multiboot_info_t *mbd) {
	map = (memory_block_t*)kernel_end;
	multiboot_memory_map_t *entry = mbd->mmap_addr;
	uint64_t curr = 0;
	while (entry < mbd->mmap_addr + mbd->mmap_length) {
		uint32_t type = entry->type;
		if (type == MULTIBOOT_MEMORY_AVAILABLE) {
			uint32_t addr_low = entry->addr_low;
			uint32_t addr_high = entry->addr_high;
			uint32_t len_low = entry->len_low;
			uint32_t len_high = entry->len_high;

			uint64_t addr = ((uint64_t) addr_high) << 32 | addr_low;
			uint64_t len = ((uint64_t) len_high) << 32 | len_low;
			
			// If the address is less than 64KiB plus the kernel end (stack + safety buffer)
			if(addr < kernel_end + 0x10000){
				size = size - (kernel_end+0x10000-addr);
				addr = kernel_end + 0x10000;
			}
			map[curr].addr = addr;
			map[curr].len = len;
			
			curr++;
		}
		entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size
				+ sizeof(entry->size));
	}
	size = curr;
	for(uint64_t i = 0; i < size; i++){
		memory_block_t block = map[i];
		if(block.addr == 0) continue;
		for(uint64_t i1 = 0; i1 < size; i1++){
			if(i1 == i) continue;
			memory_block_t block2 = map[i1];
			if(block2.addr == 0) continue;
			if(block.addr == block2.addr){
				if(block.len > block2.len){
					map[i1].len = 0;
					map[i1].addr = 0;
				}else{
					map[i].len = 0;
					map[i].addr = 0;
				}
			}
		}
	}
	kernel_end += size*sizeof(memory_block_t);
}

/* 
NOTE: don't ever use this, its only to let the paging setup work properly, after that it should NEVER be used
this function can have unintended consequences such as overwriting important memory structures
*/
void* kmalloc_ar(uint64_t sz, int align) {
	if (align && (kernel_end % align)) {
		// Align it
		kernel_end += align - (kernel_end % align);
	}
	uint64_t tmp = kernel_end;
	kernel_end += sz;
	return (void*) tmp;
}

void* kmalloc(uint64_t sz) {
	for(uint64_t i = 0; i < size; i++){
		memory_block_t block = map[i];
		if(block.len >= sz){
			uint64_t addr = block.addr;
			map[i].len -= sz;
			map[i].addr += sz;
			return (void*) addr;
		}
	}
	return -1;
}
void* kmalloc_a(uint64_t sz, int align) {
	for(uint64_t i = 0; i < size; i++){
                memory_block_t block = map[i];
		uint64_t newSz = sz + (align-block.addr%align);
                if(block.len >= sz){
                        uint64_t addr = block.addr + (align-block.addr%align);
                        block.len -= newSz;
                        block.addr += newSz;
			return (void*)addr;
                }
        }
	return -1;
}
void* kmalloc_ap(uint64_t sz, int align, uint64_t *phys) {
        for(uint64_t i = 0; i < size; i++){
                memory_block_t block = map[i];
                uint64_t newSz = sz + (align-block.addr%align);
                if(block.len >= sz){
                        uint64_t addr = block.addr + (align-block.addr%align);
                        block.len -= newSz;
                        block.addr += newSz;
			*phys = addr;
                        return (void*)addr;
                }
        }
        return -1;

}
