#include <kernel/process.h>
#include <arch/x86_64/paging.h>
#include <kernel/string.h>
#include <kernel/elf.h>
#include <kernel/fs/echfs.h>
#include <kernel/vmm.h>

context_t* create_process(char* path){
	uint64_t id = getFile(path);
        directory_entry_t *file = getFileById(id);
        uint64_t* active_directory = (uint64_t*) get_active_directory();
       	uint64_t* page_directory = (uint64_t) clone_directory(active_directory);
	 
	loaded_elf_t* elf = load_elf(file);
	switch_page_directory(page_directory);
	
	//uint64_t phys = kmalloc_p(0x200000);
	//memset((uint8_t*)phys,0,0x200000);
	
	//mapPage((uint32_t)phys,0x0,0b111);
	
	mapPages((uint32_t)elf->text_ptr,elf->text_vptr,0b101,elf->text_len);
	
	mapPages((uint32_t)elf->data_ptr,elf->data_vptr,0b111,elf->data_len);
	
	switch_page_directory(active_directory);
	context_t* context = kmalloc_p(sizeof(context_t));
	context->entry_point = elf->entry_point;
	context->status = PROCESS_RUNNABLE;
	context->page_directory = page_directory;
	return context;
}
extern void jmp_usermode();
uint64_t user_function;
extern void jump(uint64_t pointer);

void map_process(context_t* process){
	switch_page_directory(process->page_directory);
	process->status = PROCESS_RUNNING;
	//user_function = 0x0;
	//jmp_usermode();
	jump(process->entry_point);
}
