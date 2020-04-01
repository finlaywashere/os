#include <stddef.h>
#include <stdint.h>

struct FAT {
	uint16_t bytesPerSector;
	uint8_t sectorsPerCluster;
	uint16_t reservedSectors;
	uint8_t numFATs;
	uint16_t numDirs;
	uint16_t numSectors;
	uint8_t mediaDescriptor;
	uint16_t sectorsPerFAT;
	uint16_t sectorsPerTrack;
	uint16_t numHeads;
	uint32_t numHiddenSectors;
	uint32_t largeSectorCount;
	char oem[9];
};

typedef struct FAT fat16_t;

fat16_t* setupFATDisk();
void readFAT(fat16_t* fat);
