#include <kernel/pmm.h>
#include <multiboot.h>
#include <kernel/string.h>
#include <arch/x86_64/tty.h>
#define MEMORY_BLOCK_SIZE 4096

extern uint64_t _kernel_end;
uint64_t kernel_end;

struct block {
	uint64_t addr;
	uint64_t len;
};

struct block *free_addrs;
uint64_t free_addrs_size = 0;

void init_pmm(multiboot_info_t *mbd) {
	kernel_end = _kernel_end;
	free_addrs = kernel_end;
	multiboot_memory_map_t *entry = mbd->mmap_addr;
	uint64_t workingI = 0;
	while (entry < mbd->mmap_addr + mbd->mmap_length) {
		uint32_t type = entry->type;
		if (type == MULTIBOOT_MEMORY_AVAILABLE) {
			uint32_t addr_low = entry->addr_low;
			uint32_t addr_high = entry->addr_high;
			uint32_t len_low = entry->len_low;
			uint32_t len_high = entry->len_high;

			uint64_t addr = ((uint64_t) addr_high) << 32 | addr_low;
			uint64_t len = ((uint64_t) len_high) << 32 | len_low;
			if (addr < kernel_end)
				continue;

			for (uint64_t addrOffset = 0; addrOffset < len; addrOffset +=
					MEMORY_BLOCK_SIZE) {
				if (len - addrOffset < MEMORY_BLOCK_SIZE)
					break;
				free_addrs_size++;
				struct block *curr = &free_addrs[workingI];
				curr->addr = addr + addrOffset;
				curr->len = MEMORY_BLOCK_SIZE;
				free_addrs[workingI] = *curr;
				workingI++;
			}
		}
		entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size
				+ sizeof(entry->size));
	}
	kernel_end += free_addrs_size * sizeof(struct block);
}

void* pmalloc(uint64_t _size) {
	uint64_t size = _size / MEMORY_BLOCK_SIZE;
	if (_size % MEMORY_BLOCK_SIZE != 0)
		size++;
	for (uint64_t i = 0; i < free_addrs_size; i++) {
		struct block *curr = &free_addrs[i];
		if (curr->addr > kernel_end && curr->len != 0) {
			int found = 0;
			struct block *lastBlock = &curr;
			for (uint64_t j = i + 1; j < i + size; j++) {
				struct block *temp = &free_addrs[j];
				if (temp->addr != lastBlock->addr + MEMORY_BLOCK_SIZE
						|| temp->len == 0) {
					found = j;
					break;
				}
				lastBlock = &temp;
			}
			if (found != 0) {
				i = found;
				continue;
			}
			for (uint64_t j = i; j < i + size; j++) {
				struct block *temp = &free_addrs[j];
				temp->len = 0;
			}

			uint64_t addr = curr->addr;
			return (void*) addr;
		}
	}
	return 0;
}
void pfree(uint64_t addr, uint64_t _size) {
	uint64_t size = _size / MEMORY_BLOCK_SIZE;
	if (_size % MEMORY_BLOCK_SIZE != 0)
		size++;
	for (uint64_t i = 0; i < free_addrs_size; i++) {
		struct block *curr = &free_addrs[i];
		if (curr->addr == addr) {
			for (uint64_t j = i; j < i + size; j++) {
				struct block *temp = &free_addrs[j];
				temp->len = MEMORY_BLOCK_SIZE;
			}
			return;
		}
	}
}
