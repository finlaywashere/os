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
char *itoa_internal(char *buffer, size_t len, uint64_t input, int base) {
    static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *pos = buffer;
    if (input >= base)
        pos = itoa_internal(buffer, len, input/base, base);

    if (pos-buffer < len-1)
        *pos++ = digits[input % base];
    return pos;
}

char *itoa(char *buffer, size_t len, uint64_t input, int base) {
    char *pos = buffer;

    if (base < 2 || base > 36 || len < 1)
        return 0;

    if (input < 0)
        *pos++ = '-';

    pos = itoa_internal(pos, len, iabs(input), base);
    *pos = '\0';
    return buffer;
}
void terminal_writeint(uint64_t integer, int base){
	char* buffer = (char*) pmalloc(65);
	buffer = itoa(buffer,64,integer,base);
	terminal_writestring(buffer);
	pfree(buffer,65);
}
