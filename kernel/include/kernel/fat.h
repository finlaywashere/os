#include <stddef.h>
#include <stdint.h>

struct FAT {
	char bootCode[3];
	char oem[8];
	uint16_t bytesPerSector;
	uint8_t sectorsPerCluster;
	uint16_t reservedSectors;
	uint8_t numFATs;
	uint16_t numDirs;
	uint16_t numSectors;
	uint8_t mediaDescriptor;
	uint16_t _sectorsPerFAT;
	uint16_t sectorsPerTrack;
	uint16_t numHeads;
	uint32_t numHiddenSectors;
	uint32_t largeSectorCount;
	uint32_t sectorsPerFAT;
	uint16_t flags;
	uint8_t versionMajor;
	uint8_t versionMinor;
	uint32_t rootCluster;
	uint16_t fsinfoSector;
	uint16_t backupBootSector;
	char reserved[12];
	uint8_t driveNum;
	uint8_t reserved2;
	uint8_t signature;
	uint32_t volumeID;
	char volumeLabel[11];
	char systemIdentifier[8];
} __attribute__((packed));

struct fsinfo {
	uint32_t leadSignature;
	char reserved[480];
	uint32_t freeClusterCount;
	uint32_t freeClusterStart;
	char reserved2[12];
	uint32_t trailSignature;
} __attribute__((packed));

typedef struct FAT fat16_t;
typedef struct fsinfo fsinfo_t;

fat16_t* setupFATDisk();
