#include <stdint.h>

#define PAGE_ENTRIES_START 0xFFFFFFFFD00D0000

void init_paging();
uint64_t getPhysicalAddr(uint64_t virtual);
void mapPage(uint64_t physAddress, uint64_t virtualAddress, uint8_t flags);
void mapPages(uint64_t physAddress, uint64_t virtualAddress, uint8_t flags, uint64_t size);
void unmapPage(uint64_t virtualAddr);
int isMapped(uint64_t virtual);
struct page_1_table{
	uint64_t entries[512];
}__attribute__((packed));
typedef struct page_1_table page_1_table_t;
struct page_2_table{
	uint64_t entries[512];
}__attribute__((packed));
typedef struct page_2_table page_2_table_t;
struct page_3_table{
	uint64_t entries[512];
}__attribute__((packed));
typedef struct page_3_table page_3_table_t;
struct page_directory{
	uint64_t tables[512];
}__attribute__((packed));
typedef struct page_directory page_directory_t;

uint64_t virtual_to_physical(uint64_t virtual);
uint64_t tableToAddr(uint64_t entry);
uint64_t tableToMapping(uint64_t entry);
uint64_t mapTable(uint64_t entry);


