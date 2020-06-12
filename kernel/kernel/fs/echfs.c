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
	allocation_table_size = (root_block-16) * sizeof(uint64_t);
	allocation_table = kmalloc_p(allocation_table_size);
	memset(allocation_table,0,allocation_table_size);
	uint32_t allocation_table_addr = 16*identity_block->bytesPerBlock;
	read(getPort(0),allocation_table_addr,0,allocation_table_size,allocation_table);
}

uint64_t num_entries_in_root(){
	return identity_block->rootLength * (identity_block->bytesPerBlock/256);
}

directory_entry_t *read_root_directory(){
	uint64_t numEntries = num_entries_in_root();
	directory_entry_t *rootDir = kmalloc_p(sizeof(directory_entry_t)*numEntries);
	HBA_PORT *port = getPort(0);
	uint64_t base_addr = root_block * identity_block->bytesPerBlock;
	for(uint64_t i = 0; i < numEntries; i++){
		read(port,base_addr+(i*sizeof(directory_entry_t)),0,sizeof(directory_entry_t),rootDir+(i*sizeof(directory_entry_t)));
	}
	return rootDir;
}
