#include "arch/x86_64/paging.h"
#include "kernel/kernel.h"
#include "kernel/pmm.h"
#include "kernel/string.h"
#include "arch/x86_64/tty.h"

page_directory_t *active_directory;

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
void* palloc(){
	uint64_t pointer = kmalloc_pag(4096,4096);
	pointer -= get_phys_base();
	return (void*)pointer;
}
void mapPage(uint64_t physical, uint64_t virtual, uint8_t flags) {
	uint64_t p4_index = (virtual >> 39) & 0b111111111;
	uint64_t p3_index = (virtual >> 30) & 0b111111111;
	uint64_t p2_index = (virtual >> 21) & 0b111111111;
	uint64_t p1_index = (virtual >> 12) & 0b111111111;
	
	page_3_table_t* p3_table = (page_3_table_t*) tableToMapping(active_directory->tables[p4_index]);
	if(p3_table == 0){
		p3_table = palloc();
		memset(p3_table,0,4096);
		active_directory->tables[p4_index] = (((uint64_t)p3_table)) | 0b11;
	}
	page_2_table_t* p2_table = (page_2_table_t*) (p3_table->entries[p3_index] & 0xFFFFFFFFFFFFF800);
	if(p2_table == 0){
                p2_table = palloc();
		memset(p2_table,0,4096);
		p3_table->entries[p3_index] = (((uint64_t)p2_table)) | 0b11;
	}
	page_1_table_t* p1_table = (page_1_table_t*) (p2_table->entries[p2_index] & 0xFFFFFFFFFFFFF800);
	if(p1_table == 0){
                p1_table = palloc();
		memset(p1_table,0,4096);
		p2_table->entries[p2_index] = (((uint64_t)p1_table)) | 0b11;
	}
	
	uint64_t entry = physical | 0b1 | flags;
	
	p1_table->entries[p1_index] = entry;
	
	flush_tlb_single(virtual);
}

int isMapped(uint64_t virtual){
	uint64_t p4_index = virtual >> 39 & 0b111111111;
        uint64_t p3_index = virtual >> 30 & 0b111111111;
        uint64_t p2_index = virtual >> 21 & 0b111111111;
        uint64_t p1_index = virtual >> 12 & 0b111111111;

        page_3_table_t* p3_table = (page_3_table_t*) tableToMapping(active_directory->tables[p4_index]);
        if(p3_table == 0){
        	return 0;
	}
        page_2_table_t* p2_table = (page_2_table_t*) tableToMapping(p3_table->entries[p3_index]);
        if(p2_table == 0){
        	return 0;
	}
        page_1_table_t* p1_table = (page_1_table_t*) tableToMapping(p2_table->entries[p2_index]);
        if(p1_table == 0){
        	return 0;
	}
	if(p1_table->entries[p1_index] & 0x1){
		return 1;
	}
	return 0;
}

void unmapPage(uint64_t virtual) {
	uint64_t p4_index = virtual >> 39 & 0b111111111;
        uint64_t p3_index = virtual >> 30 & 0b111111111;
        uint64_t p2_index = virtual >> 21 & 0b111111111;
        uint64_t p1_index = virtual >> 12 & 0b111111111;

        page_3_table_t* p3_table = (page_3_table_t*) tableToMapping(active_directory->tables[p4_index]);
        if(p3_table == 0){
        	return;
	}
        page_2_table_t* p2_table = (page_2_table_t*) tableToMapping(p3_table->entries[p3_index]);
        if(p2_table == 0){
		return;        
	}
        page_1_table_t* p1_table = (page_1_table_t*) tableToMapping(p2_table->entries[p2_index]);
        if(p1_table == 0){
        	return;
	}
	
	p1_table->entries[p1_index] = 0;
	
        flush_tlb_single(virtual);

}
uint64_t offset = 0;
uint64_t __attribute__ ((noinline)) tableToMapping(uint64_t entry){
	uint64_t pointer = entry & 0xFFFFFFFFFFFFF000;
	pointer |= offset;
	return pointer;
}
void init_paging(){
	uint64_t cr3;
	asm ("mov %%cr3, %0": "=r"(cr3));
	active_directory = cr3;
	
	uint64_t max_mem = total_memory()*0x100000;
	mapPages(0x0,0xFFFFFFFFC0000000,1<<1,max_mem);
	offset = 0xFFFFFFFFC0000000;
	active_directory = (page_directory_t*)((uint64_t)active_directory | 0xFFFFFFFFC0000000);
	active_directory->tables[0] = 0x0;
}
