#include <arch/x86_64/paging.h>
#include <kernel/pmm.h>

void* kmalloc_page(uint64_t sz, int align){
	uint64_t pointer = kmalloc_pag(sz, align);
	mapPages(pointer-get_phys_base(),pointer,1<<1,sz);
	return (void*)pointer;
}
void* kmalloc_p(uint64_t sz){
	uint64_t pointer = kmalloc(sz);
	mapPages(pointer-get_phys_base(),pointer,1<<1,sz);
	return (void*)pointer;
}
void* kmalloc_pa(uint64_t sz, int align){
	uint64_t pointer = kmalloc_a(sz,align);
	mapPages(pointer-get_phys_base(),pointer,1<<1,sz);
	return (void*)pointer;
}
void* kmalloc_pap(uint64_t sz, int align, uint64_t *phys){
	uint64_t pointer = kmalloc_ap(sz,align,phys);
	mapPages(pointer-get_phys_base(),pointer,1<<1,sz);
	return (void*)pointer;
}
