#include "kernel/disk.h"

#include "arch/x86/io.h"
#include "kernel/pmm.h"
#include "arch/x86/tty.h"

#define IO_PORT_BASE 0x1F0
#define IO_CONTROL_BASE 0x3F6

void ataSetup(){
	outb(IO_PORT_BASE,0xA0);
}

uint16_t* readDiskData(uint32_t lba, unsigned char count, uint32_t slavebit){
        uint16_t *data = (uint16_t* ) allocPBlock(4096);
	outb(IO_PORT_BASE+6, 0xE0 | (slavebit << 4) | ((lba >> 24) & 0x0F));
	outb(IO_PORT_BASE+2, count);
	outb(IO_PORT_BASE+3,(unsigned char) lba);
	outb(IO_PORT_BASE+4,(unsigned char) (lba >> 8));
	outb(IO_PORT_BASE+5,(unsigned char) (lba >> 16));
	outb(IO_PORT_BASE+7, 0x20);
	for(int i1 = 0; i1 < 4; i1++)
                        inb(IO_PORT_BASE+7);
	for(int i = 0; i < 256; i++){
		data[i] = inw(IO_PORT_BASE);
	}
	return data;
}
void writeDiskData(uint32_t lba, unsigned char count, uint32_t slavebit, uint16_t *data){
        outb(IO_PORT_BASE+6, 0xE0 | (slavebit << 4) | ((lba >> 24) & 0x0F));
        outb(IO_PORT_BASE+2, count);
        outb(IO_PORT_BASE+3,(unsigned char) lba);
        outb(IO_PORT_BASE+4,(unsigned char) (lba >> 8));
        outb(IO_PORT_BASE+5,(unsigned char) (lba >> 16));
        outb(IO_PORT_BASE+7, 0x30);
        for(int i = 0; i < 256; i++){
		outw(IO_PORT_BASE,data[i]);
		for(int i1 = 0; i1 < 4; i1++)
			inb(IO_PORT_BASE+7);
	}
}

