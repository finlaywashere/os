#include <libc/syscall.h>
#include <libc/string.h>

void print(char* message){
	size_t len = strlen(message);
	uint16_t newArray[len];
	for(uint64_t i = 0; i < len; i++){
		char c = message[i];
		newArray[i] = (uint16_t) c;
	}
	write(0,(uint64_t)&newArray,len);
}

