#include "kernel/kernel.h"

#include "multiboot.h"
#include "kernel/pmm.h"
#include "arch/x86_64/tty.h"
#include "kernel/string.h"
#include "arch/x86_64/idt.h"
#include <stdint.h>

void kernel_main(multiboot_info_t* mbd){
	terminal_initialize();
	terminal_writestring("Successfully booted with ");
	char* bootloaderName = (void*)mbd->boot_loader_name;
	terminal_writestring(bootloaderName);
	terminal_writestring("\n");
	idt_init();
	terminal_writestring("Successfully initialized IDT\n");
	init_pmm(mbd);
	void* alloc1 = pmalloc(4096);
	void* alloc2 = pmalloc(4096);
	if(alloc2-alloc1 != 0x1000){
		terminal_setcolour(VGA_COLOUR_WHITE | VGA_COLOUR_RED << 4);
		terminal_writestring("FATAL: PMM is broken!");
		return;
	}
	pfree(alloc1,4096);
	pfree(alloc2,4096);
	terminal_writestring("Successfully initialized PMM!\n");
	int i = 5/0;
	terminal_writeint(3,10);
	terminal_writeint(i,10);
}
