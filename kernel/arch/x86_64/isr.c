#include <arch/x86_64/isr.h>
#include <kernel/string.h>
#include <arch/x86_64/tty.h>
#include <arch/x86_64/io.h>

idt_entry_t idt[256];
idt_ptr_t idt_ptr;

void init_idt(){
	idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1;
	idt_ptr.base = (uint64_t) &idt;

	uint8_t masterPICMask = inb(0xA1);
	uint8_t slavePICMask = inb(0x21);
	// Remap the irq table
	outb(0x20,0x11);
	outb(0xA0,0x11);
	outb(0x21,0x20);
	outb(0xA1,0x28);
	outb(0x21,0x04);
	outb(0xA1,0x02);
	outb(0x21,0x01);
	outb(0xA1,0x01);
	outb(0x21,masterPICMask);
	outb(0xA1,slavePICMask);

	set_idt(0,(uint64_t) isr0, 0x08, 0x8E);
	set_idt(1,(uint64_t) isr1, 0x08, 0x8E);
	set_idt(2,(uint64_t) isr2, 0x08, 0x8E);
	set_idt(3,(uint64_t) isr3, 0x08, 0x8E);
	set_idt(4,(uint64_t) isr4, 0x08, 0x8E);
	set_idt(5,(uint64_t) isr5, 0x08, 0x8E);
	set_idt(6,(uint64_t) isr6, 0x08, 0x8E);
	set_idt(7,(uint64_t) isr7, 0x08, 0x8E);
	set_idt(8,(uint64_t) isr8, 0x08, 0x8E);
	set_idt(9,(uint64_t) isr9, 0x08, 0x8E);
	set_idt(10,(uint64_t) isr10, 0x08, 0x8E);
	set_idt(11,(uint64_t) isr11, 0x08, 0x8E);
	set_idt(12,(uint64_t) isr12, 0x08, 0x8E);
	set_idt(13,(uint64_t) isr13, 0x08, 0x8E);
	set_idt(14,(uint64_t) isr14, 0x08, 0x8E);
	set_idt(15,(uint64_t) isr15, 0x08, 0x8E);
	set_idt(16,(uint64_t) isr16, 0x08, 0x8E);
	set_idt(17,(uint64_t) isr17, 0x08, 0x8E);
	set_idt(18,(uint64_t) isr18, 0x08, 0x8E);
	set_idt(19,(uint64_t) isr19, 0x08, 0x8E);
	set_idt(20,(uint64_t) isr20, 0x08, 0x8E);
	set_idt(21,(uint64_t) isr21, 0x08, 0x8E);
	set_idt(22,(uint64_t) isr22, 0x08, 0x8E);
	set_idt(23,(uint64_t) isr23, 0x08, 0x8E);
	set_idt(24,(uint64_t) isr24, 0x08, 0x8E);
	set_idt(25,(uint64_t) isr25, 0x08, 0x8E);
	set_idt(26,(uint64_t) isr26, 0x08, 0x8E);
	set_idt(27,(uint64_t) isr27, 0x08, 0x8E);
	set_idt(28,(uint64_t) isr28, 0x08, 0x8E);
	set_idt(29,(uint64_t) isr29, 0x08, 0x8E);
	set_idt(30,(uint64_t) isr30, 0x08, 0x8E);
	set_idt(31,(uint64_t) isr31, 0x08, 0x8E);
	
	set_idt(32,(uint64_t) irq0, 0x08, 0x8E);
	set_idt(33,(uint64_t) irq1, 0x08, 0x8E);
	set_idt(34,(uint64_t) irq2, 0x08, 0x8E);
	set_idt(35,(uint64_t) irq3, 0x08, 0x8E);
	set_idt(36,(uint64_t) irq4, 0x08, 0x8E);
	set_idt(37,(uint64_t) irq5, 0x08, 0x8E);
	set_idt(38,(uint64_t) irq6, 0x08, 0x8E);
	set_idt(39,(uint64_t) irq7, 0x08, 0x8E);
	set_idt(40,(uint64_t) irq8, 0x08, 0x8E);
	set_idt(41,(uint64_t) irq9, 0x08, 0x8E);
	set_idt(42,(uint64_t) irq10, 0x08, 0x8E);
	set_idt(43,(uint64_t) irq11, 0x08, 0x8E);
	set_idt(44,(uint64_t) irq12, 0x08, 0x8E);
	set_idt(45,(uint64_t) irq13, 0x08, 0x8E);
	set_idt(46,(uint64_t) irq14, 0x08, 0x8E);
	set_idt(47,(uint64_t) irq15, 0x08, 0x8E);

	
	load_idt();
}
void set_idt(uint8_t number, uint64_t base, uint16_t selector, uint8_t flags){
	idt[number].baseLow = base & 0xFFFF;
	idt[number].baseMid = (base >> 16) & 0xFFFF;
	idt[number].baseHigh = (base >> 32) & 0xFFFFFFFF;

	idt[number].selector = selector;
	idt[number].flags = flags;

	idt[number].reservedIst = 0;
	idt[number].reserved = 0;
}
void isr_handler(registers_t regs){
	terminal_writestring("Interrupt: 0x");
	terminal_writeint(regs.intNo,16);
	terminal_writestring("\n");
}
isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t num, isr_t handler){
	interrupt_handlers[num] = handler;
}

void irq_handler(registers_t regs){
	if(regs.intNo >= 40){
		// Send reset signal to slave
		outb(0xA0, 0x20);
	}
	// Send reset signal to master
	outb(0x20,0x20);

	if(interrupt_handlers[regs.intNo] != 0){
		isr_t handler = interrupt_handlers[regs.intNo];
		handler(regs);
	}
}
