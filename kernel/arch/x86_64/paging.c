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
	uint64_t p4_index = (virtual >> 39) & 0b111111111;
	uint64_t p3_index = (virtual >> 30) & 0b111111111;
	uint64_t p2_index = (virtual >> 21) & 0b111111111;
	uint64_t p1_index = (virtual >> 12) & 0b111111111;
	
	page_3_table_t* p3_table = (page_3_table_t*) tableToMapping(p4_table->tables[p4_index]);
	if(p3_table == 0){
		uint64_t pointer = kmalloc_pag(4096,4096);
		mapPage(pointer,PAGE_ENTRIES_START+pointer,1<<1);
		p3_table = tableToMapping(pointer);
		memset(p3_table,0,4096);
		p4_table->tables[p4_index] = (((uint64_t)p3_table)-get_phys_base()) | 0b11;
	}
	page_2_table_t* p2_table = (page_2_table_t*) (p3_table->entries[p3_index] & 0xFFFFFFFFFFFFF800);
	if(p2_table == 0){
                uint64_t pointer = kmalloc_pag(4096,4096);
		mapPage(pointer,PAGE_ENTRIES_START+pointer,1<<1);
                p2_table = tableToMapping(pointer);
		memset(p2_table,0,4096);
		p3_table->entries[p3_index] = (((uint64_t)p2_table)-get_phys_base()) | 0b11;
	}
	page_1_table_t* p1_table = (page_1_table_t*) (p2_table->entries[p2_index] & 0xFFFFFFFFFFFFF800);
	if(p1_table == 0){
                uint64_t pointer = kmalloc_pag(4096,4096);
		mapPage(pointer,PAGE_ENTRIES_START+pointer,1<<1);
                p1_table = tableToMapping(pointer);
		memset(p1_table,0,4096);
		p2_table->entries[p2_index] = (((uint64_t)p1_table)-get_phys_base()) | 0b11;
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

        page_3_table_t* p3_table = (page_3_table_t*) tableToMapping(p4_table->tables[p4_index]);
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
uint64_t virtual_to_physical(uint64_t virtual){
	uint64_t p4_index = virtual >> 39 & 0b111111111;
        uint64_t p3_index = virtual >> 30 & 0b111111111;
        uint64_t p2_index = virtual >> 21 & 0b111111111;
        uint64_t p1_index = virtual >> 12 & 0b111111111;

        page_3_table_t* p3_table = (page_3_table_t*) tableToMapping(p4_table->tables[p4_index]);
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
        uint64_t entry = p1_table->entries[p1_index];
	
	return entry & 0xFFFFFFFFFFFF0000;
}

void unmapPage(uint64_t virtual) {
	uint64_t p4_index = virtual >> 39 & 0b111111111;
        uint64_t p3_index = virtual >> 30 & 0b111111111;
        uint64_t p2_index = virtual >> 21 & 0b111111111;
        uint64_t p1_index = virtual >> 12 & 0b111111111;

        page_3_table_t* p3_table = (page_3_table_t*) tableToMapping(p4_table->tables[p4_index]);
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
uint64_t tableToAddr(uint64_t entry){
	return entry & 0xFFFFFFFFFFFF0000;
}
uint64_t tableToMapping(uint64_t entry){
	return tableToAddr(entry) + get_phys_base();
}
uint64_t mapTable(uint64_t entry){
	uint64_t virtAddr = tableToMapping(entry);
	uint64_t phys = tableToAddr(entry);
	mapPage(phys,virtAddr,1<<1);
}
void earlyMap(uint64_t phys,uint64_t virt){
	uint64_t p4_index = virt >> 39 & 0b111111111;
        uint64_t p3_index = virt >> 30 & 0b111111111;
        uint64_t p2_index = virt >> 21 & 0b111111111;
        uint64_t p1_index = virt >> 12 & 0b111111111;
	page_3_table_t* p3_table = (page_3_table_t*) tableToAddr(p4_table->tables[p4_index]);
	page_2_table_t* p2_table = (page_2_table_t*) tableToAddr(p3_table->entries[p3_index]);
	page_1_table_t* p1_table = (page_1_table_t*) tableToAddr(p2_table->entries[p2_index]);
	p1_table->entries[p1_index] = phys | 0b11;
	
	flush_tlb_single(virt);
}

void init_paging() {
	uint64_t cr3;
	asm ("mov %%cr3, %0": "=r"(cr3));
	p4_table = cr3;
	
	uint64_t pointer_3 = kmalloc_pag(4096,4096);
	uint64_t pointer_2 = kmalloc_pag(4096,4096);
	uint64_t pointer_1 = kmalloc_pag(4096,4096);
	
	page_3_table_t *p3_table = (page_3_table_t*)pointer_3;
	p3_table->entries[511] = (pointer_2-get_phys_base()) | 0b11;

	page_2_table_t *p2_table = (page_2_table_t*)pointer_2;
	p2_table->entries[128] = (pointer_1-get_phys_base()) | 0b11;
	
	page_1_table_t *p1_table = (page_1_table_t*)pointer_1;
	uint64_t p1_index = pointer_3 >> 12 & 0b111111111;
	p1_table->entries[p1_index] = (pointer_3-get_phys_base()) | 0b11;
	p1_index = pointer_2 >> 12 & 0b111111111;
	p1_table->entries[p1_index] = (pointer_2-get_phys_base()) | 0b11;
	p1_index = pointer_1 >> 12 & 0b111111111;
	p1_table->entries[p1_index] = (pointer_1-get_phys_base()) | 0b11;
	
	
	
	mapTable(p4_table->tables[511]);
	
	p4_table->tables[0] = 0x0;
	
	mapPages(get_page_phys_base(),get_page_base(),1<<1,0x100000);
	mapPages(get_kernel_end()-get_phys_base(),get_kernel_end(),1<<1,0x400000);
}
