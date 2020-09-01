#include <kernel/syscall.h>

void init_syscalls(){
	register_interrupt_handler(SYSCALL, &exec_syscall);
}

void exec_syscall(registers_t regs){
	uint64_t code = regs.rax;
	if(code == 0){
		uint16_t val = (uint16_t) regs.rbx;
		if(val>>8)
			terminal_setcolour((uint8_t)val>>8);
		terminal_putchar((uint8_t) val);
	}else if(code == 1){
		uint64_t newPos = regs.rbx;
		uint32_t xPos = (uint32_t) (newPos>>32);
		uint32_t yPos = (uint32_t) newPos;
		terminal_setpos((size_t)yPos,(size_t)xPos);
	}else if(code == 2){
		regs.rbx = terminal_getpos();
	}
}
