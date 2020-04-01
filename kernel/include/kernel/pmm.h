#include "multiboot.h"
#include "utils.h"

void initPMM(multiboot_info_t* mbd);
void* malloc(uint64_t _size);
void free(uint64_t addr, uint64_t _size);
