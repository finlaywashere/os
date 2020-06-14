#include <multiboot.h>
#include <stdint.h>

struct memory_block{
	uint64_t addr;
	uint64_t len;
};
typedef struct memory_block memory_block_t;

void init_pmm_base();
void init_pmm(multiboot_info_t* mbd);
void* kmalloc(uint64_t sz);
void* kmalloc_a(uint64_t sz, int align);
void* kmalloc_ap(uint64_t sz, int align, uint64_t *phys);
void* kmalloc_ar(uint64_t sz, int align);
void *kmalloc_pag(uint64_t size, int align);
uint64_t get_page_base();
uint64_t get_page_phys_base();
uint64_t get_phys_base();
uint64_t get_kernel_end();

