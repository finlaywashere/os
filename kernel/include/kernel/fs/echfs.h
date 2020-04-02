#include <stdint.h>
#include <stddef.h>
#include <arch/x86/tty.h>
#include <arch/x86/disk.h>
#include <kernel/pmm.h>

struct bs {
	char jump[4];
	char signature[8];
	uint64_t blockCount;
	uint64_t rootLength;
	uint64_t bytesPerBlock;
}__attribute__((packed));

struct directory_entry {
	uint64_t directoryID;
	uint8_t type;
	char name[201];
	uint64_t accessTime;
	uint64_t modifiedTime;
	uint16_t permissions;
	uint16_t ownerID;
	uint16_t groupID;
	uint64_t creationTime;
	uint64_t startingBlock;
	uint64_t size;
}__attribute__((packed));

typedef struct bs echfs_bs_t;
typedef struct directory_entry echfs_dir_entry_t;

struct echfs {
	echfs_bs_t* bs;
	uint64_t* allocTable;
};

typedef struct echfs echfs_t;

echfs_t init_echfs();
