#include <kernel/syscall.h>

void init_syscalls(){
	register_interrupt_handler(SYSCALL, &exec_syscall);
}

void exec_syscall(registers_t regs){
	terminal_putchar(regs.rax);
}
