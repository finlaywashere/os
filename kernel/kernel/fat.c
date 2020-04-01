#include "kernel/fat.h"
#include "arch/x86/disk.h"
#include "kernel/pmm.h"
#include "arch/x86/tty.h"
#include "kernel/string.h"

fat16_t* setupFATDisk(){
	uint16_t* data = readDiskData(0, 18, 0);
	uint8_t* byteData = (uint8_t* ) malloc(36);
	for(int i = 0; i < 18; i++){
		uint16_t d = data[i];
		byteData[i*2+1] = d >> 8;
		byteData[i*2] = d & 0x00ff;
	}	

	fat16_t* fs = (fat16_t *) malloc(34);
	
	for(int i = 0; i < 8; i++){
		char data2 = byteData[i+3];
		fs->oem[i] = data2;
	}
	fs->oem[8] = 0;
	
	uint16_t bytesPerSector = (uint16_t)(((uint16_t)byteData[12]) << 8) | (0x00ff & byteData[11]);
	fs->bytesPerSector = bytesPerSector;
	
	uint8_t sectorsPerCluster = (uint8_t) byteData[13];
	fs->sectorsPerCluster = sectorsPerCluster;
	
	uint16_t sectorsPerFAT = (uint16_t)(((uint16_t)byteData[23]) << 8) | (0x00ff & byteData[22]);
	fs->sectorsPerFAT = sectorsPerFAT;
	
	uint8_t numFATs = byteData[16];
	fs->numFATs = numFATs;
	
	uint16_t numDirectories = (uint16_t)(((uint16_t)byteData[17]) << 8) | (0x00ff & byteData[18]);
	fs->numDirs = numDirectories;
	
	uint16_t totalSectors = (uint16_t)(((uint16_t)byteData[19]) << 8) | (0x00ff & byteData[20]);
	fs->numSectors = totalSectors;
	
	if(totalSectors == 0){
		char* array[4];
		array[3] = byteData[32];
		array[2] = byteData[33];
		array[1] = byteData[34];
		array[0] = byteData[35];
		uint32_t extendedSectors = *(uint32_t *)array;
		fs->largeSectorCount = extendedSectors;
	}
	
	uint8_t mediaType = byteData[21];
	fs->mediaDescriptor = mediaType;

	uint16_t sectorsPerTrack = (uint16_t)(((uint16_t)byteData[24]) << 8) | (0x00ff & byteData[25]);
	fs->sectorsPerTrack = sectorsPerTrack;
	
	uint16_t numHeads = (uint16_t)(((uint16_t)byteData[26]) << 8) | (0x00ff & byteData[27]);
	fs->numHeads = numHeads;

	char* hiddenSectors[4];
	hiddenSectors[3] = byteData[28];
	hiddenSectors[2] = byteData[29];
	hiddenSectors[1] = byteData[30];
	hiddenSectors[0] = byteData[31];
	uint32_t numHiddenSectors = *(uint32_t *)hiddenSectors;
	fs->numHiddenSectors = numHiddenSectors;

	uint16_t numReservedSectors = (uint16_t)(((uint16_t)byteData[14]) << 8) | (0x00ff & byteData[15]);
	fs->reservedSectors = numReservedSectors;
	
	return fs;
}


