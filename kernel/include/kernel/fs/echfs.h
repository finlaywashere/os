#include <stdint.h>
struct identity_block{
	uint8_t jmp[4];
	uint8_t sig[8];
	uint64_t numBlocks;
	uint64_t rootLength;
	uint64_t bytesPerBlock;
}__attribute__((packed));
typedef struct identity_block identity_block_t;

struct directory_entry{
	uint64_t dirID;
	uint8_t objType;
	uint8_t name[201];
	uint64_t atime;
	uint64_t mtime;
	uint16_t permissions;
	uint16_t ownerID;
	uint16_t groupID;
	uint64_t ctime;
	uint64_t startBlock;
	uint64_t fileSize;
}__attribute__((packed));
typedef struct directory_entry directory_entry_t;
void init_echfs();
identity_block_t* identity_block;
uint64_t num_entries_in_root();
directory_entry_t *read_root_directory();
