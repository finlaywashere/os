#include <stdint.h>

void* kmalloc_p(uint64_t sz);
void* kmalloc_pa(uint64_t sz, int align);
void* kmalloc_pap(uint64_t sz, int align, uint64_t *phys);
void* kmalloc_page(uint64_t sz, int align);


