#include <kernel/syscall.h>
#include <arch/x86_64/tty.h>
#include <kernel/process.h>

void init_syscalls(){
	register_interrupt_handler(SYSCALL, &exec_syscall);
}

registers_t exec_syscall(registers_t regs){
	regs.rsp += 0x8;
	uint64_t code = regs.rax;
	if(code == 0){
		uint8_t* buf = (uint16_t*) regs.rbx;
		uint64_t len = regs.rcx;
		uint64_t descriptor = regs.rdx; // TODO: Implement different descriptors
		uint64_t start = (uint64_t) buf;
		uint64_t end = ((uint64_t)buf) + len;
		if(start >= 0xffff800000000000 || end >= 0xffff800000000000 || start < 0 || end < 0)
			return;
		if(descriptor == 0)
			terminal_write(buf,len);
	}else if(code == 1){
		// Exit
		regs = *process_exit(&regs);
		return regs;
	}else if(code == 2){
		// Exec
		uint8_t* buf = (uint8_t*) regs.rbx;
                uint64_t len = strlen(buf);
		uint64_t start = (uint64_t) buf;
                uint64_t end = ((uint64_t)buf) + len;
                if(start >= 0xffff800000000000 || end >= 0xffff800000000000 || start < 0 || end < 0)
                        return;
		save_process(&regs);
		pause_process();
		context_t* process = create_process(buf);
		map_process(process);
		return process->state;
	}
}
