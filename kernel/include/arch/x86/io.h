#include "utils.h"

#define IO_PORT_BASE 0x1F0
#define IO_CONTROL_BASE 0x3F6

void outb(u16int port, u8int value);
u8int inb(u16int port);
