#include <kernel/syscall.h>
#include <arch/x86_64/tty.h>
#include <kernel/process.h>

void init_syscalls(){
	register_interrupt_handler(SYSCALL, &exec_syscall);
}

registers_t exec_syscall(registers_t regs){
	//regs.rsp += 0x8;
	uint64_t code = regs.rax;
	if(code == WRITE){
		uint16_t* buf = (uint16_t*) regs.rbx;
		uint64_t len = regs.rcx;
		uint8_t newBuf[len];
		for(int i = 0; i < len; i++){
			newBuf[i] = buf[i];
		}
		uint64_t descriptor = regs.rdx; // TODO: Implement different descriptors
		uint64_t start = (uint64_t) buf;
		uint64_t end = ((uint64_t)buf) + len;
		if(start >= 0xffff800000000000 || end >= 0xffff800000000000 || start < 0 || end < 0){
			regs.rax = 1;
			return regs;
		}
		if(descriptor == 0){
			terminal_write(&newBuf,len);
			regs.rax = 0;
			return regs;
		}
		regs.rax = 1;
		return regs;
	}else if(code == READ){
		uint16_t* buf = (uint16_t*) regs.rbx;
		uint64_t descriptor = regs.rdx;
		uint64_t len = regs.rcx;
		uint64_t start = (uint64_t) buf;
                uint64_t end = ((uint64_t)buf) + len;
                if(start >= 0xffff800000000000 || end >= 0xffff800000000000 || start < 0 || end < 0){
                        regs.rax = 1;
                        return regs;
                }
		if(descriptor == 0){
			// Any length above 32 is invalid
			if(len > 32 || len < 0){
				regs.rax = 2;
				return regs;
			}
			for(int i = 0; i < len; i++){
				uint16_t key = getCurrKey();
				if(key == 0x0)
					break;
				nextKey();
				buf[i] = key;
			}
			regs.rax = 0;
			return regs;
		}
	}else if(code == EXIT){
		// Exit
		regs = *process_exit(&regs);
		regs.rax = 0;
		return regs;
	}else if(code == EXEC){
		// Exec
		uint8_t* buf = (uint8_t*) regs.rbx;
                uint64_t len = regs.rcx;
		uint64_t start = (uint64_t) buf;
                uint64_t end = ((uint64_t)buf) + len;
                if(start >= 0xffff800000000000 || end >= 0xffff800000000000 || start < 0 || end < 0){
                        regs.rax = 1;
			return regs;
		}
		save_process(&regs);
		create_process(buf);
		regs.rax = 0;
		return regs;
	}
}
