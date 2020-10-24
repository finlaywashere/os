#include <stdint.h>
#include <stddef.h>

extern void write(uint64_t,uint64_t,uint64_t);
extern void exit_asm();
extern void exec_asm(uint64_t);

void print(char* message);
void exit();
void exec();
