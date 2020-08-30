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
#include "arch/x86_64/mbr.h"
#include "arch/x86_64/acpi.h"
#include "kernel/process.h"

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
	init_acpi();
	terminal_writestring("Successfully initialized ACPI\n");
	init_keyboard();
	terminal_writestring("Successfully initialized keyboard\n");
	mbr_table_t* mbr = init_mbr();
	terminal_writestring("MBR signature: 0x");
	uint16_t mbr_sig = mbr->bootsectorSignature;
	terminal_writeint(mbr_sig & 0xFF,16);
	terminal_writestring(" - 0x");
	terminal_writeint(mbr_sig>>8,16);
	terminal_writestring("\nSuccessfully initialized MBR\n");
	init_pci();
	terminal_writestring("Successfully initialized PCI\n");
	init_ahci();
	terminal_writestring("Successfully initialized AHCI\n");
	init_echfs();
	echfs_setup_fs_map();
	terminal_writestring("Successfully initialized ECHFS!\n");
	context_t* process = create_process("test.bin");
	terminal_writestring("Successfully loaded ELF file from disk!\n");
	terminal_writestring("Entering process");
	map_process(process);
	terminal_writestring("Process returned");
	
	while(1){
		// Kernel loop
	}
}
