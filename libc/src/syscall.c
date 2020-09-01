#include <libc/syscall.h>
#include <libc/string.h>

void print(char* message){
	size_t len = strlen(message);
	for(size_t i = 0; i < len; i++){
		uint8_t character = message[i];
		screen_putchar((uint64_t) character);
	}
}

