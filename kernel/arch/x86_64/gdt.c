#include <arch/x86_64/gdt.h>
#include <kernel/vmm.h>

extern gdt_t GDT64;

gdt_t *newGDT;

gdt_pointer_t gdt_ptr;

extern void load_gdt();
extern void load_tss();

extern tss_t tss_mem;
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
	uint64_t tss_ptr = &tss_mem+0xffff800000000000;
	tss_t *tss = (tss_t*) tss_ptr;
	
	newGDT->tss.limit_low = sizeof(tss_t);
	newGDT->tss.base_low = tss_ptr & 0xFFFF;
	newGDT->tss.base_mid = (tss_ptr >> 16) & 0xFF;
	newGDT->tss.base_high = (tss_ptr >> 24) & 0xFF;
	newGDT->tss.base_higher = (tss_ptr >> 32);
	load_tss();
}
