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
	init_pci();
	terminal_writestring("Successfully initialized PCI\n");
	init_ahci();
	terminal_writestring("Successfully initialized AHCI\n");
	uint16_t *buf = kmalloc(1024);
	memset(buf,0,1024);
	int result = read(getPort(0), 0, 0, 1024, buf);
	uint8_t *newBuf = (uint8_t *)buf;
	if(result){
		terminal_writestring("Failed to read from disk 0!\n");
	}else{
		terminal_writestring("Successfully read from disk 0!\n");
		for(int i = 4; i < 12; i++){
			terminal_putchar(newBuf[i]);
		}
		terminal_putchar('\n');
	}
	while(1){
		// Kernel loop
	}
}
