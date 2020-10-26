#include <stdint.h>
#include <stddef.h>

extern uint64_t syscall_asm(uint64_t rax,uint64_t rbx,uint64_t rcx, uint64_t rdx);

void print(char* message);
void exit();
void exec();
uint16_t readCharacter();
