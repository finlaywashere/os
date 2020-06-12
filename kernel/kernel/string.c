#include <kernel/string.h>
#include "arch/x86_64/tty.h"
#include "kernel/pmm.h"

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void *memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}
unsigned iabs(int input) {
    if (input >= 0)
        return (unsigned)input;
    return (4294967295 - (unsigned)input)+1U;
}
char *itoa_internal(char *buffer, uint64_t len, uint64_t input, uint64_t base) {
    static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *pos = buffer;
    if (input >= base)
        pos = itoa_internal(buffer, len, input/base, base);

    if ((uint64_t)(pos-buffer) < len-1)
        *pos++ = digits[input % base];
    return pos;
}

char *itoa(char *buffer, size_t len, uint64_t input, int base) {
    char *pos = buffer;

    if (base < 2 || base > 36 || len < 1)
        return 0;

    pos = itoa_internal(pos, (uint64_t) len, iabs(input), base);
    *pos = '\0';
    return buffer;
}
void terminal_writeint(uint64_t integer, int base){
	char* buffer = (char*) kmalloc_p(65);
	buffer = itoa(buffer,64,integer,base);
	terminal_writestring(buffer);
}
char* append(char* c1, char* c2){
	char* ret = kmalloc_p(strlen(c1)+strlen(c2));
	uint64_t i = 0;
	while(c1[i]){
		ret[i] = c1[i];
		i++;
	}
	uint64_t len = i;
	i = 0;
	while(c2[i]){
		ret[i+len] = c2[i];
		i++;
	}
	return ret;
}

int count(char* str, char delim){
	int count = 0;
        for(int i = 0; i < strlen(str); i++){
                if(str[i] == delim)
                        count++;
        }
	return count;
}

