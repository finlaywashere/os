#include <libc/syscall.h>
#include <libc/string.h>
#include <kernel/syscall.h>

void print(char* message){
	size_t len = strlen(message);
	uint16_t buf[len];
	for(int i = 0; i < len; i++){
		buf[i] = message[i];
	}
	syscall_asm(WRITE,(uint64_t)&buf,len,0);
}
void exit(){
	syscall_asm(EXIT,1,0,0);
}
void exec(char* file){
	size_t len = strlen(file);
	syscall_asm(EXEC,(uint64_t) file,len,0);
}
