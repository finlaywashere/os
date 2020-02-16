#include "kernel/fat.h"
#include "kernel/disk.h"
#include "kernel/pmm.h"
#include "arch/x86/tty.h"
#include "kernel/string.h"

void setupFATDisk(){
	uint16_t* data = readDiskData(0, 18, 0);
	uint8_t* byteData = (uint8_t* )allocPBlock(36);
	for(int i = 0; i < 18; i++){
		uint16_t d = data[i];
		byteData[i*2+1] = d >> 8;
		byteData[i*2] = d & 0x00ff;
	}
	
	char* oem = (char* )allocPBlock(8);
	terminal_writestring("OEM: ");
	for(int i = 0; i < 8; i++){
		char* data = byteData[i+3];
		oem[i] = data;
		terminal_putchar(oem[i]);
	}
	
	uint32_t bytesPerSector = (uint32_t)(((uint16_t)byteData[11]) << 8) | (0x00ff & byteData[12]);
	char* buffer = (char* ) allocPBlock(1024);
	for(int i = 0; i < 1024; i++){
                buffer[i] = 0;
        }
	
	buffer = itoa(bytesPerSector,buffer,10);
	terminal_writestring("\nBytes per sector: ");
	terminal_writestring((uint8_t)(bytesPerSector+48));
	
	for(int i = 0; i < 1024; i++){
		buffer[i] = 0;
	}
	
	uint8_t numFATS = byteData[16];
	terminal_writestring("\nNum fats: ");
	terminal_putchar((uint8_t) (numFATS+48));	
	
	uint32_t numDirectories = (uint32_t)(((uint16_t)byteData[17]) << 8) | (0x00ff & byteData[18]);
	buffer = itoa(numDirectories,buffer,10);
	terminal_writestring("\nNum dirs: ");
	terminal_writestring(buffer);
	
	for(int i = 0; i < 1024; i++){
                buffer[i] = 0;
        }
	
	uint32_t totalSectors = (uint32_t)(((uint16_t)byteData[19]) << 8) | (0x00ff & byteData[20]);
	if(totalSectors == 0){
		char* array[4];
		array[3] = byteData[32];
		array[2] = byteData[33];
		array[1] = byteData[34];
		array[0] = byteData[35];
		totalSectors = *(uint32_t *)array;
	}
	buffer = itoa(totalSectors,buffer,10);
	terminal_writestring("\nNum sectors: ");
	terminal_writestring(buffer);
}
