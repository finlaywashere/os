#include "kernel/paging.h"
#include "kernel/kernel.h"
#include "kernel/pmm.h"
#include "kernel/string.h"
#include "arch/x86_64/tty.h"

page_directory_t *p4_table;

#define PAGE_SIZE 0x200000

uint64_t getPhysicalAddr(uint64_t virtual) {
	uint64_t pdindex = virtual >> 22;
	uint64_t ptindex = (virtual >> 12) & 0x03FF;

	page_table_t *pt = p4_table->tables[pdindex];

	return (pt->entries[ptindex] & ~0xFFF) + (virtual & 0xFFF);
}

static inline void flush_tlb_single(unsigned long addr){
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void mapPage(uint64_t physical, uint64_t virtual, uint8_t flags) {
	uint64_t pdindex = virtual >> 22;
	uint64_t ptindex = (virtual >> 12) & 0x03FF;

	page_table_t *pt = p4_table->tables[pdindex];
	if (pt->entries[ptindex]) {
		panic("Tried to map page to an address that is already mapped");
	}
	pt->entries[ptindex] = physical | (flags & 0xFFF) | 0x01; //Present
	flush_tlb_single(virtual);
}

void unmapPage(uint64_t virtual) {
	uint64_t pdindex = virtual >> 22;
	uint64_t ptindex = (virtual >> 12) & 0x03FF;

	page_table_t *pt = p4_table->tables[pdindex];
	if (!pt->entries[ptindex]) {
		return;
	}
	pt->entries[ptindex] = 0x0; // Unmap
}

void init_paging() {
	uint64_t cr3;
	asm ("mov %%cr3, %0": "=r"(cr3));
	p4_table = cr3;
	for(int i = 0; i < 512; i++){
		if(!p4_table->tables[i]){
			page_table_t *pt = kmalloc_a(sizeof(page_table_t),1);
			uint64_t pointer = pt;
			uint64_t entry = (pointer & 0xFFFFFFFFFFFF0000) | 0x3;
			p4_table->tables[i] = entry;
		}
	}
}
