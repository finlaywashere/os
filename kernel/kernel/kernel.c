#include "kernel/kernel.h"

#include "arch/x86/tty.h"
#include "utils.h"
#include "multiboot.h"
#include "kernel/string.h"

void kernel_main(multiboot_info_t* mbd, unsigned int magic){
	terminal_initialize();
	char *buffer[32];
	itoa(magic,buffer,16);
	terminal_writestring(buffer);
}
