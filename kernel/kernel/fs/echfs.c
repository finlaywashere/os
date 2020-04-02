#include <kernel/fs/echfs.h>

echfs_t init_echfs(){
	uint16_t* data = readDiskData(0, 512, 0);
	echfs_bs_t* bs = data;
	uint64_t* allocTable = (uint64_t* ) malloc(sizeof(uint64_t)*bs->blockCount);
	
	echfs_t* fs = (echfs_t* ) malloc(sizeof(echfs_t));

	fs->bs = bs;
	fs->allocTable = allocTable;
	
	uint64_t sectorCount = bs->blockCount/bs->bytesPerBlock;
	if(bs->blockCount%bs->bytesPerBlock != 0)
		sectorCount++;
	
	int entriesPerSector = bs->bytesPerBlock/sizeof(uint64_t);
	for(uint64_t i = 0; i < sectorCount; i++){
		uint64_t* entries = readDiskData(16+i*bs->bytesPerBlock,512,0);
		for(int i1 = 0; i1 < entriesPerSector; i1++){
			if(i*entriesPerSector+i1 > bs->blockCount){
				break;
			}
			allocTable[i*entriesPerSector+i1] = entries[i1];
		}
	}
	uint64_t rootStart = 16*bs->bytesPerBlock+(bs->blockCount*sizeof(uint64_t));
	uint8_t *data2 = readDiskData(rootStart,bs->rootLength*bs->bytesPerBlock,0);
	terminal_writeint(data2[1],16);
	echfs_dir_entry_t *dirs = data2;
	for(uint64_t i = 0; i < bs->rootLength; i++){
		echfs_dir_entry_t dir = dirs[i];
		terminal_writestring("\nRoot addr: ");
		terminal_writeint(rootStart,10);
		terminal_writestring("\nDirectory id: ");
		terminal_writeint(dir.directoryID,16);
		terminal_writestring("\nDirectory name: ");
		terminal_writeint((uint32_t)dir.name[0],10);
		break;
	}
}
