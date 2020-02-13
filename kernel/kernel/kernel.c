#include "kernel/kernel.h"

#include "arch/x86/tty.h"
#include "utils.h"

void kernel_main(void){
	terminal_initialize();
	terminal_writestring("Hello world!");
}
