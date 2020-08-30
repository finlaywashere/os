#include <stddef.h>
#include <stdint.h>

void* memset(void* memory, int value, size_t count);
size_t strlen(const char* str);
char *itoa(char *buffer, size_t len, uint64_t input, int base);
void terminal_writeint(uint64_t integer, int base);
char* append(char* c1, char* c2);
int count(char* str, char delim);
int strcmp(char *str1, char* str2);
int memcmp(char* str1, char* str2, int len);


