#include <arch/x86_64/pit.h>
#include "kernel/kernel.h"

#include "multiboot.h"
#include "kernel/pmm.h"
#include "arch/x86_64/tty.h"
#include "kernel/string.h"
#include <stdint.h>
#include "arch/x86_64/isr.h"
#include "kernel/paging.h"
//#include "arch/x86_64/keyboard.h"
#include "kernel/timer.h"

void panic(char *message){
	asm volatile("cli");
	terminal_writestring("PANIC: ");
	terminal_writestring(message);
	while(1);
}

void kernel_main(multiboot_info_t* mbd){
	terminal_initialize();
	terminal_writestring("Successfully booted with ");
	char* bootloaderName = (void*) (uint64_t) mbd->boot_loader_name;
	terminal_writestring(bootloaderName);
	terminal_writestring("\n");
	init_idt();
	terminal_writestring("Successfully initialized IDT\n");
	init_pmm(mbd);
	terminal_writestring("Successfully initialized PMM\n");
	init_paging();
	terminal_writestring("Successfully initialized paging\n");
	init_timer();
	terminal_writestring("Successfully initialized PIT\n");
	init_keyboard();
	terminal_writestring("Successfully initialized keyboard\n");

	while(1){
		// Kernel loop
	}
}
