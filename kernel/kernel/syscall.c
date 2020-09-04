#include <kernel/syscall.h>
#include <arch/x86_64/tty.h>

void init_syscalls(){
	register_interrupt_handler(SYSCALL, &exec_syscall);
}

void exec_syscall(registers_t regs){
	uint64_t code = regs.rax;
	if(code == 0){
		uint16_t* buf = (uint16_t*) regs.rbx;
		uint64_t len = regs.rcx;
		uint64_t descriptor = regs.rdx; // TODO: Implement different descriptors
		uint64_t start = (uint64_t) buf;
		uint64_t end = ((uint64_t)buf) + len;
		if(start >= 0xffff800000000000 || end >= 0xffff800000000000 || start < 0 || end < 0)
			return;
		if(descriptor == 0)
			terminal_writec(buf,len);
	}
}
