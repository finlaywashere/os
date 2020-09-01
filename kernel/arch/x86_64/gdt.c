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
tss_t *tss;
void init_tss(){
	uint64_t tss_ptr = ((uint64_t)&tss_mem)|0xffff800000000000;
	tss = (tss_t*) tss_ptr;
	
	newGDT->tss.limit_low = sizeof(tss_t);
	newGDT->tss.base_low = tss_ptr & 0xFFFF;
	newGDT->tss.base_mid = (tss_ptr >> 16) & 0xFF;
	newGDT->tss.base_high = (tss_ptr >> 24) & 0xFF;
	newGDT->tss.base_higher = (tss_ptr >> 32);
	
	set_kernel_stack(getrsp());
	
	load_tss();
}
void set_kernel_stack(uint64_t stack){
	tss->rsp0_low = (uint32_t) stack;
	tss->rsp0_high = stack>>32;
}
