#include <kernel/process.h>
#include <arch/x86_64/paging.h>
#include <kernel/string.h>
#include <kernel/elf.h>
#include <kernel/fs/echfs.h>
#include <kernel/vmm.h>

context_t* create_process(char* path){
	uint64_t id = getFile(path);
        directory_entry_t *file = getFileById(id);
	uint64_t size = file->fileSize+0x200000;
	uint64_t* page_directory = (uint64_t*)palloc();
        uint64_t* active_directory = (uint64_t*)get_active_directory();
        memset(page_directory,0,256*sizeof(uint64_t));
        for(int i = 127; i < 256; i++){
                page_directory[i] = active_directory[i];
        }
        
	uint64_t address = load_elf(file);
	switch_page_directory(page_directory);
        mapPages((uint32_t)address,0,0b11,size);
	
	switch_page_directory(active_directory);
	context_t* context = kmalloc_p(sizeof(context_t));
	context->status = PROCESS_RUNNABLE;
	context->page_directory = page_directory;
	return context;
}
void map_process(context_t* process){
	switch_page_directory(process->page_directory);
	process->status = PROCESS_RUNNING;
	elf_header_t* elf_header = (elf_header_t*) 0x0;
        terminal_writestring(elf_header->magic);
	asm volatile("jmp 0");
}
