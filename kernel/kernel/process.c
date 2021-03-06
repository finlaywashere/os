#include <kernel/process.h>
#include <arch/x86_64/paging.h>
#include <kernel/string.h>
#include <kernel/elf.h>
#include <kernel/fs/echfs.h>
#include <kernel/vmm.h>
#include <arch/x86_64/isr.h>

context_t* processes;
int currProcess = 1;

void init_processes(){
	processes = kmalloc_p(sizeof(context_t)*10);
	memset(processes,0,sizeof(context_t)*10);
}
int runningProcess = -1;
registers_t* schedule(registers_t* regs){
	save_process(regs);
	if(processes[runningProcess].status == PROCESS_RUNNING){
		processes[runningProcess].state = *regs;
        	processes[runningProcess].status = PROCESS_RUNNABLE;
	}
	int newProcess = 0;
	for(int i = runningProcess+1; i <= runningProcess+10;i++){
		int status = processes[i%10].status;
		if(status == PROCESS_RUNNABLE){
			newProcess = i%10;
			break;
		}
	}
	runningProcess = newProcess;
	map_process(&processes[runningProcess]);
	return &processes[runningProcess].state;
	//jump(processes[runningProcess].state.rip,processes[runningProcess].state);
}
registers_t* process_exit(registers_t *regs){
	processes[runningProcess].status = PROCESS_STOPPED;
	terminal_writestring("Process ");
	terminal_writeint(runningProcess,10);
	terminal_writestring(" exited!");
	return schedule(regs);
}
void pause_process(){
	processes[runningProcess].status = PROCESS_RUNNABLE;
}
void save_process(registers_t *regs){
	if(runningProcess == -1)
		return;
	processes[runningProcess].state = *regs;
}

context_t* create_process(char* path){
	uint64_t id = getFile(path);
        directory_entry_t *file = getFileById(id);
	uint64_t* active_directory = (uint64_t*) get_active_directory();
        uint64_t* boot_directory = (uint64_t*) get_boot_directory();
       	uint64_t* page_directory = (uint64_t) clone_directory(boot_directory);
	 
	loaded_elf_t* elf = load_elf(file);
	uint64_t stack = kmalloc_p(16384);
	switch_page_directory(page_directory);
	
	//uint64_t phys = kmalloc_p(0x200000);
	//memset((uint8_t*)phys,0,0x200000);
	
	//mapPage((uint32_t)phys,0x0,0b111);
	
	mapPages((uint32_t)elf->text_ptr,elf->text_vptr,0b101,elf->text_len);
	mapPages((uint32_t)stack,0xFFF00000,0b111,16384);
	
	if(elf->data_len != 0)
		mapPages((uint32_t)elf->data_ptr,elf->data_vptr,0b111,elf->data_len);
	
	switch_page_directory(active_directory);
	context_t* context = kmalloc_p(sizeof(context_t));
	memset(context,0,sizeof(context_t));
	context->entry_point = elf->entry_point;
	context->status = PROCESS_RUNNABLE;
	context->page_directory = page_directory;
	context->state.rip = elf->entry_point;
	context->state.userrsp = 0xFFF00000;
	context->state.cs = 0x1b;
	register uint64_t rsp asm ("rsp");
	if(currProcess == 0)
		context->state.rsp = rsp-0x18;
	else
		context->state.rsp = rsp-0x8;
	context->state.ds = 0x23;
	context->state.ss = 0x23;
	context->state.eflags = 0b1000000010;
	
	if(currProcess > 9)
		panic("No processes left!");
	processes[currProcess] = *context;
	currProcess++;
	return context;
}

void map_process(context_t* process){
	uint64_t active_directory = get_active_directory;
	switch_page_directory(process->page_directory);
	process->status = PROCESS_RUNNING;
	//user_function = 0x0;
	//jmp_usermode();
	//jump(process->entry_point,process->state);
	//switch_page_directory(active_directory);
}
