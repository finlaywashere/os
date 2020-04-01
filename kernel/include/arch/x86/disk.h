#include "utils.h"

void ataSetup();
void writeDiskData(uint32_t lba, unsigned char count, uint32_t slavebit, uint16_t *data);
uint16_t* readDiskData(uint32_t lba, uint8_t* count, uint32_t slavebit);
