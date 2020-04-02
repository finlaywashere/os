#include "kernel/kernel.h"

#include "arch/x86/tty.h"
#include "utils.h"
#include "multiboot.h"
#include "kernel/string.h"
#include "kernel/pmm.h"
#include "arch/x86/isr.h"

void panic(char* message){
	terminal_writestring(message);
	while(1){}
}

extern uint64_t *_kernel_end;

void kernel_main(multiboot_info_t* mbd, unsigned int magic){
	terminal_initialize();
	if(magic != 0x2BADB002){
		panic("Wrong magic!");
	}
	isr_install();
	char* bootloaderName = (void*)mbd->boot_loader_name;
        terminal_writestring("Successfully booted with ");
        terminal_writestring(bootloaderName);
	terminal_writestring("\n");
	
	initPMM(mbd);
	ataSetup();
	
	init_echfs();	
}
