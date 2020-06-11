#include "kernel/paging.h"
#include "kernel/kernel.h"
#include "kernel/pmm.h"
#include "kernel/string.h"
#include "arch/x86_64/tty.h"

page_directory_t *p4_table;

#define PAGE_SIZE 0x1000

static inline void flush_tlb_single(unsigned long addr){
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void mapPage(uint64_t physical, uint64_t virtual, uint8_t flags) {
	uint64_t p4_index = virtual >> 39;
	uint64_t p3_index = virtual >> 30 & 0b000000000111111111;
	uint64_t p2_index = virtual >> 21 & 0b000000000000000000111111111;
	uint64_t p1_index = virtual >> 12 & 0b000000000000000000000000000111111111;
	
	page_3_table_t* p3_table = (page_3_table_t*) (p4_table->tables[p4_index] & 0xFFFFFFFFFFFFF800);
	if(p3_table == 0){
		p3_table = kmalloc_a(4096,4096);
		memset(p3_table,0,4096);
		p4_table->tables[p4_index] = (uint64_t)p3_table | 0b11;
	}
	page_2_table_t* p2_table = (page_2_table_t*) (p3_table->entries[p3_index] & 0xFFFFFFFFFFFFF800);
	if(p2_table == 0){
		p2_table = kmalloc_a(4096,4096);
		memset(p2_table,0,4096);
		p3_table->entries[p3_index] = (uint64_t)p2_table | 0b11;
	}
	page_1_table_t* p1_table = (page_1_table_t*) (p2_table->entries[p2_index] & 0xFFFFFFFFFFFFF800);
	if(p1_table == 0){
		p1_table = kmalloc_a(4096,4096);
		memset(p1_table,0,4096);
		p2_table->entries[p2_index] = (uint64_t)p1_table | 0b11;
	}
	
	if(p1_table->entries[p1_index] != 0){
		panic("Tried to map page where page exists!");
	}
	
	uint64_t entry = physical | 0b1 | flags;
	
	p1_table->entries[p1_index] = entry;
	
	flush_tlb_single(virtual);
}

void unmapPage(uint64_t virtual) {
	return;
}

void init_paging() {
	uint64_t cr3;
	asm ("mov %%cr3, %0": "=r"(cr3));
	p4_table = cr3;
}
