#include "kernel/fat.h"
#include "arch/x86/disk.h"
#include "kernel/pmm.h"
#include "arch/x86/tty.h"
#include "kernel/string.h"

fat16_t* setupFATDisk(){
	uint16_t* data1 = readDiskData(0, 512, 0);
	fat16_t* fs = data1;
	
	uint16_t* data = readDiskData(fs->fsinfoSector*fs->bytesPerSector,512,0);
	fsinfo_t* fsinfo = data;

	terminal_writestring("\nAvailable clusters: ");
	terminal_writeint(fsinfo->freeClusterCount,10);	
	terminal_writestring("\nFree clusters start: ");
	terminal_writeint(fsinfo->freeClusterStart,10);
	terminal_writestring("\nRoot cluster: ");
	terminal_writeint(fs->rootCluster,10);
	
	uint8_t FAT_table[fs->bytesPerSector];
	uint32_t fat_offset = fs->rootCluster * 4;
	uint32_t fat_sector = fs->numHiddenSectors + (fat_offset / fs->bytesPerSector);
	uint32_t ent_offset = fat_offset % fs->bytesPerSector;

	terminal_writestring("\nFAT sector: ");
	terminal_writeint(fat_sector,10);
	
	readDiskData(fat_sector*fs->bytesPerSector,512,0);

	uint32_t table_value = *(uint16_t*)&FAT_table[ent_offset]; 
	terminal_writestring("\nTable value: ");
	terminal_writeint(table_value,10);

	return fs;
}
