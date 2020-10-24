#include <stdint.h>
#include <stddef.h>
#include <arch/x86_64/isr.h>

#define PROCESS_NULL 0
#define PROCESS_RUNNABLE 1
#define PROCESS_KILLED 2
#define PROCESS_RUNNING 3
#define PROCESS_STOPPED 4

struct context{
	registers_t state;
	uint16_t status;
	uint64_t *page_directory;
	uint64_t entry_point;
};
typedef struct context context_t;

//extern void jmp_usermode();
//uint64_t user_function;
//extern void jump(uint64_t pointer, registers_t registers);

context_t* create_process(char* path);
void map_process(context_t* process);
registers_t* schedule(registers_t* regs);
void init_processes();
void process_exit(registers_t *regs);
