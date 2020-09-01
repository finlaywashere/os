#include <stdint.h>
#include <stddef.h>
#include <arch/x86_64/isr.h>

#define PROCESS_RUNNABLE 1;
#define PROCESS_KILLED 2;
#define PROCESS_RUNNING 3;
#define PROCESS_STOPPED 4;

struct context{
	registers_t state;
	uint16_t status;
	uint64_t *page_directory;
	uint64_t entry_point;
};
typedef struct context context_t;

context_t* create_process(char* path);
void map_process(context_t* process);
