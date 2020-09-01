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
        uint64_t* active_directory = (uint64_t*) get_active_directory();
       	uint64_t* page_directory = (uint64_t) clone_directory(active_directory);
	 
	uint64_t address = load_elf(file);
	switch_page_directory(page_directory);
        mapPages((uint32_t)address,0,0b111,size);
	
	switch_page_directory(active_directory);
	context_t* context = kmalloc_p(sizeof(context_t));
	context->status = PROCESS_RUNNABLE;
	context->page_directory = page_directory;
	return context;
}
extern void jmp_usermode();
uint64_t user_function;
void map_process(context_t* process){
	switch_page_directory(process->page_directory);
	process->status = PROCESS_RUNNING;
	//user_function = 0x0;
	//jmp_usermode();
	asm("jmp 0x0");
}
