#include <multiboot.h>
#include <stdint.h>

void init_pmm(multiboot_info_t* mbd);
void* pmalloc(uint64_t size);
void pfree(uint64_t addr, uint64_t size);
