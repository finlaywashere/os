#include <kernel/pmm.h>
#include <multiboot.h>
#include <kernel/string.h>
#include <arch/x86_64/tty.h>

#define KERNEL_VMA 0xffff800000000000

extern uint64_t _kernel_virtual_end;
extern uint64_t _kernel_virtual_start;
uint64_t kernel_end;
uint64_t kernel_phys_end;

memory_block_t *map;
uint64_t size;
uint64_t page_base;
uint64_t page_curr;
uint64_t max_mem;

uint64_t get_page_base(){
	return page_base;
}
uint64_t get_page_phys_base(){
	return page_base-KERNEL_VMA;
}
uint64_t get_phys_base(){
	return KERNEL_VMA;
}
uint64_t get_kernel_end(){
	return kernel_end;
}

void init_pmm_base(multiboot_info_t *mbd){
	max_mem = (mbd->mem_upper<<32) | mbd->mem_lower;
	kernel_end = &_kernel_virtual_end;
        kernel_end += 0xA10000;
	page_base = kernel_end;
	page_curr = page_base;
	kernel_end += 0x100000;
	kernel_phys_end = kernel_end-KERNEL_VMA;
}
void *kmalloc_pag(uint64_t size, int align){
	if(page_curr % align > 0){
		page_curr += align-page_curr%align;
	}
	uint64_t tmp = page_curr;
	page_curr += size;
	return (void*)tmp;
}
uint64_t total_memory(){
	return max_mem;
}
void init_pmm(multiboot_info_t *mbd) {
	/*map = (memory_block_t*)kernel_end;
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
			if(addr < kernel_phys_end + 0x10000){
				if(size < (size - (kernel_phys_end+0x10000-addr))){
					entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size
                                + sizeof(entry->size));
					continue;
				}
				size = size - (kernel_phys_end+0x10000-addr);
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
	for(uint64_t i = 0; i < size; i++){
		memory_block_t block = map[i];
		// Reserve the very last block for paging or else
		if(block.len >= 0x100000){
			uint64_t addr = block.addr;
			map[size].addr = addr;
			map[size].len = 0x100000;
			map[i].addr += 0x100000;
			map[i].len -= 0x100000;
			break;
		}
	}
	size++;
	kernel_end += size*sizeof(memory_block_t);*/
}

void* kmalloc(uint64_t sz) {
	uint64_t tmp = kernel_end;
	kernel_end += sz;
	return tmp;
	/*
	for(uint64_t i = 0; i < size-1; i++){
		memory_block_t block = map[i];
		if(block.len >= sz){
			uint64_t addr = block.addr;
			map[i].len -= sz;
			map[i].addr += sz;
			return (void*) addr;
		}
	}
	return -1;*/
}
void* kmalloc_a(uint64_t sz, int align) {
	if(kernel_end % align > 0){
		kernel_end += align-(kernel_end % align);
	}
	uint64_t tmp = kernel_end;
	kernel_end += sz;
	return tmp;
	
	/*for(uint64_t i = 0; i < size-1; i++){
                memory_block_t block = map[i];
		uint64_t newSz = sz + (align-block.addr%align);
                if(block.len >= sz){
                        uint64_t addr = block.addr + (align-block.addr%align);
                        map[i].len -= newSz;
                        map[i].addr += newSz;
			return (void*)addr;
                }
        }
	return -1;*/
}
void* kmalloc_ap(uint64_t sz, int align, uint64_t *phys) {
        if(kernel_end % align > 0){
                kernel_end += align-(kernel_end % align);
        }
        uint64_t tmp = kernel_end;
        phys = tmp;
	kernel_end += sz;
        return tmp;

	/*for(uint64_t i = 0; i < size-1; i++){
                memory_block_t block = map[i];
                uint64_t newSz = sz + (align-block.addr%align);
                if(block.len >= sz){
                        uint64_t addr = block.addr + (align-block.addr%align);
                        map[i].len -= newSz;
                        map[i].addr += newSz;
			*phys = addr;
                        return (void*)addr;
                }
        }
        return -1;*/

}
