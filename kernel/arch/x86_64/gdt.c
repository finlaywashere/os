#include <arch/x86_64/gdt.h>
#include <kernel/vmm.h>

extern gdt_t GDT64;

gdt_t *newGDT;

gdt_pointer_t gdt_ptr;

extern void load_gdt();
extern void load_tss();

void init_gdt(){
	newGDT = (uint64_t)&GDT64 | 0xffff800000000000;
	newGDT->pointer.addr |= 0xffff800000000000;
	gdt_ptr = newGDT->pointer;
	load_gdt();
}
uint64_t getrsp(){
	__asm__ ("movq %rsp, %rax");
}
void init_tss(){
	tss_t *tss = (tss_t*)kmalloc_p(sizeof(tss_t));
	memset(tss,0,sizeof(tss));
	
	
	
	newGDT->entries[5].limit_low = sizeof(tss_t);
	uint32_t addr = (uint32_t) tss;
	newGDT->entries[5].base_low = addr & 0xFFFF;
	newGDT->entries[5].base_mid = (addr >> 16) & 0xFF;
	newGDT->entries[5].base_high = addr >> 24;
	load_tss();
}
