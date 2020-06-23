#include <arch/x86_64/pit.h>
#include "kernel/kernel.h"

#include "multiboot.h"
#include "kernel/pmm.h"
#include "arch/x86_64/tty.h"
#include "kernel/string.h"
#include <stdint.h>
#include "arch/x86_64/isr.h"
#include "arch/x86_64/paging.h"
#include "arch/x86_64/ahci.h"
#include "kernel/timer.h"
#include "kernel/fs/echfs.h"
void panic(char *message){
	asm volatile("cli");
	terminal_writestring("PANIC: ");
	terminal_writestring(message);
	while(1);
}

void kernel_main(multiboot_info_t* mbd){
	mbd = (multiboot_info_t*) (((uint64_t)mbd)+0xffff800000000000);
	terminal_initialize();
	terminal_writestring("Successfully booted with ");
	char* bootloaderName = (void*) (uint64_t) mbd->boot_loader_name;
	terminal_writestring(bootloaderName);
	terminal_writestring("\n");
	init_gdt();
	terminal_writestring("Successfully initialized GDT\n");
	//init_idt();
	terminal_writestring("Successfully initialized IDT\n");
	init_pmm_base(mbd);
        terminal_writestring("Successfully initialized PMM base\n");
	init_pmm(mbd);
	terminal_writestring("Successfully initialized PMM full\n");
	init_paging();
	terminal_writestring("Successfully initialized paging\n");
	init_timer();
	terminal_writestring("Successfully initialized PIT\n");
	init_keyboard();
	terminal_writestring("Successfully initialized keyboard\n");
	init_pci();
	terminal_writestring("Successfully initialized PCI\n");
	init_ahci();
	terminal_writestring("Successfully initialized AHCI\n");
	init_echfs();
	echfs_setup_fs_map();
	terminal_writestring("Successfully initialized ECHFS!\n");
	load_elf();
	terminal_writestring("Successfully loaded ELF file from disk!\n");

	while(1){
		// Kernel loop
	}
}
