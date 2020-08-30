#include <kernel/elf.h>
#include <kernel/fs/echfs.h>
#include <kernel/vmm.h>

uint64_t load_elf(directory_entry_t* file){
	terminal_writestring("ELF name: ");
	terminal_writestring(file->name);
	terminal_writestring("\n");
	
	uint8_t *buf = (uint8_t*)kmalloc_pa(file->fileSize,0x200000);
	readFile(file,buf);
	
	return buf;
}
