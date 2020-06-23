#include <kernel/elf.h>
#include <kernel/fs/echfs.h>

#define PROGRAM_BASE 0x0

void load_elf(){
	uint64_t id = getFile("test.bin");
	directory_entry_t *file = getFileById(id);
	terminal_writestring("ELF name: ");
	terminal_writestring(file->name);
	terminal_writestring("\n");
	
	uint8_t *buf = kmalloc_pa(file->fileSize,0x200000);
	readFile(file,buf);
	
	mapPages(toPhysical((uint64_t)buf),PROGRAM_BASE,1<<1,file->fileSize);
	elf_header_t* elf_header = (elf_header_t*) 0x0;
	terminal_writestring(elf_header->magic);
}
