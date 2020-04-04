#include <multiboot.h>
#include <stdint.h>

void init_pmm(multiboot_info_t* mbd);
void* kmalloc(uint64_t sz);
void* kmalloc_a(uint64_t sz, int align);
void* kmalloc_ap(uint64_t sz, int align, uint64_t *phys);
