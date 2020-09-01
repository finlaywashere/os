#include <stdint.h>
#include <stddef.h>

extern void screen_putchar(uint64_t);
extern void screen_setpos(uint64_t);
extern uint64_t screen_getpos();

void print(char* message);

