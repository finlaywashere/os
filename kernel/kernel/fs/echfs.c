#include <kernel/fs/echfs.h>
#include <arch/x86_64/ahci.h>
#include <kernel/vmm.h>
uint64_t allocation_table_size;
uint64_t root_block;
uint64_t *allocation_table;
void init_echfs(){
	identity_block = kmalloc_p(sizeof(identity_block_t));
	read(getPort(0),0,0,sizeof(identity_block_t),identity_block);
	terminal_writestring(identity_block->sig);
	terminal_writestring(" - bytes per block: ");
	terminal_writeint(identity_block->bytesPerBlock,10);
	terminal_writestring("\n");
	
	root_block = (identity_block->numBlocks * sizeof(uint64_t) + identity_block->bytesPerBlock - 1)/identity_block->bytesPerBlock + 16;
	allocation_table_size = (identity_block->numBlocks * sizeof(uint64_t) + identity_block->bytesPerBlock - 1)/ identity_block->bytesPerBlock * sizeof(uint64_t);
	allocation_table = kmalloc_p(allocation_table_size);
	memset(allocation_table,0,allocation_table_size);
	read(getPort(0),16,0,allocation_table_size/sizeof(uint64_t),allocation_table);
}

uint64_t num_entries_in_root(){
	return identity_block->rootLength * (identity_block->bytesPerBlock/256);
}

directory_entry_t *read_root_directory(){
	uint64_t numEntries = num_entries_in_root();
	directory_entry_t *rootDir = kmalloc_p(sizeof(directory_entry_t)*numEntries);
	HBA_PORT *port = getPort(0);
	uint64_t newNumEntries = numEntries/2;
	for(uint64_t i = 0; i < newNumEntries; i++){
		read(port,root_block+i,0,1,rootDir+(i*sizeof(directory_entry_t)*2));
	}
	return rootDir;
}
uint8_t *readFile(directory_entry_t *file){
	uint8_t *file_buf = kmalloc_p(file->fileSize);
	uint64_t sector = file->startBlock;
	HBA_PORT *port = getPort(0);
	int curr = 0;
	while(1){
		uint64_t alloc_val = allocation_table[sector];
		
		read(port,sector,0,1,file_buf+(curr*identity_block->bytesPerBlock));
		curr++;
		
		if(alloc_val > 0 && alloc_val < 0xFFFFFFFFFFFFFFEF){
			sector = alloc_val;
		}else{
			// Done or error
			break;
		}
	}
	return file_buf;
}
