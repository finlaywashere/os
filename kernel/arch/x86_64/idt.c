#include <arch/x86_64/idt.h>
#include <arch/x86_64/io.h>
#include <arch/x86_64/tty.h>
#include <kernel/string.h>

struct IDT_entry {
	uint16_t offset_lowerbits;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_higherbits;
	uint32_t offset_3;
	uint32_t zero2;
};

struct IDT_entry IDT[256];
extern int load_idt();
extern int irq0();
extern int irq1();
extern int irq2();
extern int irq3();
extern int irq4();
extern int irq5();
extern int irq6();
extern int irq7();
extern int irq8();
extern int irq9();
extern int irq10();
extern int irq11();
extern int irq12();
extern int irq13();
extern int irq14();
extern int irq15();

/* ISRs reserved for CPU exceptions */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

unsigned long idt_address;
unsigned long idt_ptr[2];

void idt_init(void) {

	/* remapping the PIC */
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 40);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);

	add_idt_entry((uint64_t) isr0, 0);
	add_idt_entry((uint64_t) isr1, 1);
	add_idt_entry((uint64_t) isr2, 2);
	add_idt_entry((uint64_t) isr3, 3);
	add_idt_entry((uint64_t) isr4, 4);
	add_idt_entry((uint64_t) isr5, 5);
	add_idt_entry((uint64_t) isr6, 6);
	add_idt_entry((uint64_t) isr7, 7);
	add_idt_entry((uint64_t) isr8, 8);
	add_idt_entry((uint64_t) isr9, 9);
	add_idt_entry((uint64_t) isr10, 10);
	add_idt_entry((uint64_t) isr11, 11);
	add_idt_entry((uint64_t) isr12, 12);
	add_idt_entry((uint64_t) isr13, 13);
	add_idt_entry((uint64_t) isr14, 14);
	add_idt_entry((uint64_t) isr15, 15);
	add_idt_entry((uint64_t) isr16, 16);
	add_idt_entry((uint64_t) isr17, 17);
	add_idt_entry((uint64_t) isr18, 18);
	add_idt_entry((uint64_t) isr19, 19);
	add_idt_entry((uint64_t) isr20, 20);
	add_idt_entry((uint64_t) isr21, 21);
	add_idt_entry((uint64_t) isr22, 22);
	add_idt_entry((uint64_t) isr23, 23);
	add_idt_entry((uint64_t) isr24, 24);
	add_idt_entry((uint64_t) isr25, 25);
	add_idt_entry((uint64_t) isr26, 26);
	add_idt_entry((uint64_t) isr27, 27);
	add_idt_entry((uint64_t) isr28, 28);
	add_idt_entry((uint64_t) isr29, 29);
	add_idt_entry((uint64_t) isr30, 30);
	add_idt_entry((uint64_t) isr31, 31);

	add_idt_entry((uint64_t) irq0,32);
	add_idt_entry((uint64_t) irq1,33);
	add_idt_entry((uint64_t) irq2,34);
	add_idt_entry((uint64_t) irq3,35);
	add_idt_entry((uint64_t) irq4,36);
	add_idt_entry((uint64_t) irq5,37);
	add_idt_entry((uint64_t) irq6,38);
	add_idt_entry((uint64_t) irq7,39);
	add_idt_entry((uint64_t) irq8,40);
	add_idt_entry((uint64_t) irq9,41);
	add_idt_entry((uint64_t) irq10,42);
	add_idt_entry((uint64_t) irq11,43);
	add_idt_entry((uint64_t) irq12,44);
	add_idt_entry((uint64_t) irq13,45);
	add_idt_entry((uint64_t) irq14,46);
	add_idt_entry((uint64_t) irq15,47);

	/* fill the IDT descriptor */
	idt_address = (unsigned long) IDT;
	idt_ptr[0] = (sizeof(struct IDT_entry) * 256)
			+ ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address;

	load_idt(idt_ptr);

}
void add_idt_entry(uint64_t address, int index){
	IDT[index].offset_lowerbits = address & 0xffff;
	IDT[index].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[index].zero = 0;
	IDT[index].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[index].offset_higherbits = (address & 0xffff0000) >> 16;
	IDT[index].offset_3 = (address & 0xffff00000000) >> 32;
	IDT[index].zero2 = 0;
}
void irq0_handler(void) {
	terminal_writestring("Received interrupt 0!\n");
	outb(0x20, 0x20); //EOI
}

void irq1_handler(void) {
	terminal_writestring("Received interrupt 1!\n");
	outb(0x20, 0x20); //EOI
}

void irq2_handler(void) {
	terminal_writestring("Received interrupt 2!\n");
	outb(0x20, 0x20); //EOI
}

void irq3_handler(void) {
	terminal_writestring("Received interrupt 3!\n");
	outb(0x20, 0x20); //EOI
}

void irq4_handler(void) {
	terminal_writestring("Received interrupt 4!\n");
	outb(0x20, 0x20); //EOI
}

void irq5_handler(void) {
	terminal_writestring("Received interrupt 5!\n");
	outb(0x20, 0x20); //EOI
}

void irq6_handler(void) {
	terminal_writestring("Received interrupt 6!\n");
	outb(0x20, 0x20); //EOI
}

void irq7_handler(void) {
	terminal_writestring("Received interrupt 7!\n");
	outb(0x20, 0x20); //EOI
}

void irq8_handler(void) {
	terminal_writestring("Received interrupt 8!\n");
	outb(0xA0, 0x20);
	outb(0x20, 0x20); //EOI
}

void irq9_handler(void) {
	terminal_writestring("Received interrupt 9!\n");
	outb(0xA0, 0x20);
	outb(0x20, 0x20); //EOI
}

void irq10_handler(void) {
	terminal_writestring("Received interrupt 10!\n");
	outb(0xA0, 0x20);
	outb(0x20, 0x20); //EOI
}

void irq11_handler(void) {
	terminal_writestring("Received interrupt 11!\n");
	outb(0xA0, 0x20);
	outb(0x20, 0x20); //EOI
}

void irq12_handler(void) {
	terminal_writestring("Received interrupt 12!\n");
	outb(0xA0, 0x20);
	outb(0x20, 0x20); //EOI
}

void irq13_handler(void) {
	terminal_writestring("Received interrupt 13!\n");
	outb(0xA0, 0x20);
	outb(0x20, 0x20); //EOI
}

void irq14_handler(void) {
	terminal_writestring("Received interrupt 14!\n");
	outb(0xA0, 0x20);
	outb(0x20, 0x20); //EOI
}

void irq15_handler(void) {
	terminal_writestring("Received interrupt 15!\n");
	outb(0xA0, 0x20);
	outb(0x20, 0x20); //EOI
}

void isr_handler(registers_t r) {
	terminal_writestring("Exception occurred number 0x");
	char buffer[10];
	itoa(buffer,10,r.int_no,16);
	terminal_writestring(buffer);
	terminal_writestring("\n");
}
