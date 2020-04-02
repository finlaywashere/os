#include <stddef.h>
#include <stdint.h>

void* memset(void* memory, int value, size_t count);
size_t strlen(const char* str);
char *itoa(char *buffer, size_t len, uint64_t input, int base);
void terminal_writeint(uint64_t integer, int base);
