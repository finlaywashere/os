#include <stdint.h>
#include <stddef.h>
#include <arch/x86_64/isr.h>

void init_syscalls();
registers_t exec_syscall(registers_t regs);

