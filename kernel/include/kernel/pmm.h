#include "multiboot.h"
#include "utils.h"

void initPMM(multiboot_info_t* mbd);
uint64_t allocBlock(uint64_t _size);
void freeBlock(uint64_t addr, uint64_t _size);
