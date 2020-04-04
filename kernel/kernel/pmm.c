#include <kernel/pmm.h>
#include <multiboot.h>
#include <kernel/string.h>
#include <arch/x86_64/tty.h>

extern uint64_t _kernel_end;
uint64_t kernel_end;

uint64_t placement_address;

void init_pmm(multiboot_info_t *mbd) {
	kernel_end = _kernel_end;
	multiboot_memory_map_t *entry = mbd->mmap_addr;
	uint64_t highest_addr;
	uint64_t highest_len = 0;
	while (entry < mbd->mmap_addr + mbd->mmap_length) {
		uint32_t type = entry->type;
		if (type == MULTIBOOT_MEMORY_AVAILABLE) {
			uint32_t addr_low = entry->addr_low;
			uint32_t addr_high = entry->addr_high;
			uint32_t len_low = entry->len_low;
			uint32_t len_high = entry->len_high;

			uint64_t addr = ((uint64_t) addr_high) << 32 | addr_low;
			uint64_t len = ((uint64_t) len_high) << 32 | len_low;

			if (len > highest_len) {
				highest_addr = addr;
				highest_len = len;
			}
		}
		entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size
				+ sizeof(entry->size));
	}
}

void* kmalloc(uint64_t sz) {
	uint64_t tmp = placement_address;
	placement_address += sz;
	return (void*) tmp;
}
void* kmalloc_a(uint64_t sz, int align) {
	if (align && (placement_address % align)) {
		// Align it
		placement_address += align - (placement_address % align);
	}
	uint64_t tmp = placement_address;
	placement_address += sz;
	return (void*) tmp;
}
void* kmalloc_ap(uint64_t sz, int align, uint64_t *phys) {
	if (align && (placement_address % align)) {
		// Align it
		placement_address += align - (placement_address % align);
	}
	if(phys){
		*phys = placement_address;
	}

	uint64_t tmp = placement_address;
	placement_address += sz;
	return (void*) tmp;
}
