#include "kernel/kernel.h"

#include "multiboot.h"
#include "kernel/pmm.h"
#include "arch/x86_64/tty.h"
#include "kernel/string.h"
#include <stdint.h>
#include "arch/x86_64/isr.h"
#include "arch/x86_64/pit.h"
#include "kernel/paging.h"

void panic(char *message){
	asm volatile("cli");
	terminal_writestring("PANIC: ");
	terminal_writestring(message);
	while(1);
}

void kernel_main(multiboot_info_t* mbd){
	terminal_initialize();
	terminal_writestring("Successfully booted with ");
	char* bootloaderName = (void*)mbd->boot_loader_name;
	terminal_writestring(bootloaderName);
	terminal_writestring("\n");
	init_idt();
	terminal_writestring("Successfully initialized IDT\n");
	init_pmm(mbd);
	terminal_writestring("Successfully initialized PMM\n");
	init_paging();
	terminal_writestring("Successfully initialized paging\n");
	init_timer(60);
	terminal_writestring("Successfully initialized PIT\n");
	while(1){
		// Kernel loop
	}
}
