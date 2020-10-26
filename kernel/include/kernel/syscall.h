#include <stdint.h>
#include <stddef.h>
#include <arch/x86_64/isr.h>

#define WRITE 0
#define READ 1
#define EXIT 2
#define EXEC 3

void init_syscalls();
registers_t exec_syscall(registers_t regs);

