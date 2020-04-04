#include <stdint.h>

void init_paging();
uint64_t getPhysicalAddr(uint64_t virtual);
void mapPage(uint64_t physAddress, uint64_t virtualAddress, uint8_t flags);
void unmapPage(uint64_t virtual);

struct page_table{
	uint64_t entries[1024];
}__attribute__((packed));
typedef struct page_table page_table_t;

struct page_directory{
	page_table_t *tables[1024];
}__attribute__((packed));
typedef struct page_directory page_directory_t;
