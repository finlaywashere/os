#include <arch/x86_64/gdt.h>

extern gdt_t GDT64;

gdt_t *newGDT;

gdt_pointer_t gdt_ptr;

extern void load_gdt();

void init_gdt(){
	newGDT = (uint64_t)&GDT64 | 0xFFFFFFFFC0000000;
	newGDT->pointer.addr |= 0xFFFFFFFFC0000000;
	gdt_ptr = newGDT->pointer;
	load_gdt();
}
