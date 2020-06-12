#include "arch/x86_64/paging.h"
#include "kernel/kernel.h"
#include "kernel/pmm.h"
#include "kernel/string.h"
#include "arch/x86_64/tty.h"

page_directory_t *p4_table;

#define PAGE_SIZE 0x1000

static inline void flush_tlb_single(unsigned long addr){
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}
void mapPages(uint64_t physical, uint64_t virtual, uint8_t flags, uint64_t size){
	uint64_t numPages = size/PAGE_SIZE+1;
	for(uint64_t i = 0; i < numPages; i++){
		if(!isMapped(virtual+(PAGE_SIZE*i)))
			mapPage(physical+(PAGE_SIZE*i),virtual+(PAGE_SIZE*i),flags);
	}
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
	
	uint64_t entry = physical | 0b1 | flags;
	
	p1_table->entries[p1_index] = entry;
	
	flush_tlb_single(virtual);
}

int isMapped(uint64_t virtual){
	uint64_t p4_index = virtual >> 39;
        uint64_t p3_index = virtual >> 30 & 0b000000000111111111;
        uint64_t p2_index = virtual >> 21 & 0b000000000000000000111111111;
        uint64_t p1_index = virtual >> 12 & 0b000000000000000000000000000111111111;

        page_3_table_t* p3_table = (page_3_table_t*) (p4_table->tables[p4_index] & 0xFFFFFFFFFFFFF800);
        if(p3_table == 0){
        	return 0;
	}
        page_2_table_t* p2_table = (page_2_table_t*) (p3_table->entries[p3_index] & 0xFFFFFFFFFFFFF800);
        if(p2_table == 0){
        	return 0;
	}
        page_1_table_t* p1_table = (page_1_table_t*) (p2_table->entries[p2_index] & 0xFFFFFFFFFFFFF800);
        if(p1_table == 0){
        	return 0;
	}
	if(p1_table->entries[p1_index] & 0x1){
		return 1;
	}
	return 0;
}

void unmapPage(uint64_t virtual) {
	uint64_t p4_index = virtual >> 39;
        uint64_t p3_index = virtual >> 30 & 0b000000000111111111;
        uint64_t p2_index = virtual >> 21 & 0b000000000000000000111111111;
        uint64_t p1_index = virtual >> 12 & 0b000000000000000000000000000111111111;

        page_3_table_t* p3_table = (page_3_table_t*) (p4_table->tables[p4_index] & 0xFFFFFFFFFFFFF800);
        if(p3_table == 0){
        	return;
	}
        page_2_table_t* p2_table = (page_2_table_t*) (p3_table->entries[p3_index] & 0xFFFFFFFFFFFFF800);
        if(p2_table == 0){
		return;        
	}
        page_1_table_t* p1_table = (page_1_table_t*) (p2_table->entries[p2_index] & 0xFFFFFFFFFFFFF800);
        if(p1_table == 0){
        	return;
	}
	
	p1_table->entries[p1_index] = 0;
	
        flush_tlb_single(virtual);

}

void init_paging() {
	uint64_t cr3;
	asm ("mov %%cr3, %0": "=r"(cr3));
	p4_table = cr3;
	
	mapPages(0x200000,0x200000,1<<1,0x200000);
}
