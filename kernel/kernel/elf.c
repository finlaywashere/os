#include <kernel/elf.h>
#include <kernel/fs/echfs.h>
#include <kernel/vmm.h>

loaded_elf_t* load_elf(directory_entry_t* file){
	uint8_t *buf = (uint8_t*)kmalloc_pa(file->fileSize,0x200000);
	readFile(file,buf);
	
	elf_header_t *header = (elf_header_t*)buf;
	char* magic = header->magic;
	int invalid = 0;
	if(magic[0] != 0x7F)
		invalid = 1;
	if(magic[1] != 'E')
		invalid = 2;
	if(magic[2] != 'L')
		invalid = 3;
	if(magic[3] != 'F')
		invalid = 4;
	if(invalid)
		panic("Invalid ELF header!");
	loaded_elf_t* elf = kmalloc_p(sizeof(loaded_elf_t));
	
	elf->entry_point = header->entryPosition;
		
	uint16_t headerLength = header->numHeaderEntries;
	uint64_t headerPosition = header->headerTablePosition;
	
	for(uint16_t i = 0; i < headerLength; i++){
		elf_program_header_t* p_header = (elf_program_header_t*)(buf+headerPosition+sizeof(elf_program_header_t)*i);
		if(p_header->segmentType == 0)
			continue;
		uint64_t length = p_header->sizeInMemory;
		uint64_t align = p_header->align;
		uint64_t v_addr = p_header->memOffset;
		
		uint64_t cpy_mem_offset = v_addr%0x200000;
		
		uint8_t* section = kmalloc_pa(length+0x200000,align);
		memset(section,0,length);
		
		uint64_t f_length = p_header->sizeInFile;
		uint64_t f_offset = p_header->dataOffset;

		uint8_t* offset_buf = (uint8_t*)(buf+f_offset);
		uint8_t* offset_section = (uint8_t*)(section+cpy_mem_offset);
		memcpy(offset_buf,offset_section,f_length);
		uint32_t flags = p_header->flags;
		if(flags & 1){
			// Code segment
			elf->text_ptr = (uint64_t) section;
			elf->text_vptr = v_addr;
			elf->text_len = length;
		}else if(flags & 2){
			// Data segment
			elf->data_ptr = (uint64_t) section;
                        elf->data_vptr = v_addr;
                        elf->data_len = length;

		}
		
	}
	
	return elf;
}
