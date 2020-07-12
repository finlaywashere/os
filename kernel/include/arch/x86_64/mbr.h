#include <stdint.h>
struct mbr_entry{
	uint8_t attributes;
	char chsStart[3];
	uint8_t type;
	char chsEnd[3];
	uint32_t lbaStart;
	uint32_t numSectors;
}__attribute__((packed));
typedef struct mbr_entry mbr_entry_t;
struct mbr_table{
	char bootstrap[440];
	char signature[4];
	uint16_t reserved;
	mbr_entry_t entries[4];
	uint16_t bootsectorSignature;
}__attribute__((packed));
typedef struct mbr_table mbr_table_t;

mbr_table_t* init_mbr();
