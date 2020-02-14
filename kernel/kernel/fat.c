#include "kernel/fat.h"
#include "kernel/disk.h"
#include "kernel/pmm.h"
#include "arch/x86/tty.h"

void setupFATDisk(){
	uint16_t* data = readDiskData(0, 18, 0);
	uint8_t* byteData = (uint8_t* )allocPBlock(36);
	for(int i = 0; i < 18; i++){
		uint16_t d = data[i];
		byteData[i*2] = d >> 8;
		byteData[i*2+1] = d & 0x00ff;
	}
	terminal_writestring("\n");
	for(int i = 0; i < 36; i++){
		terminal_putchar(byteData[i]);
	}
}
