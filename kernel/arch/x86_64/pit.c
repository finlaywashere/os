#include "../../include/arch/x86_64/pit.h"

#include <arch/x86_64/tty.h>
#include <kernel/string.h>
#include <arch/x86_64/io.h>
#include <arch/x86_64/isr.h>
#include <kernel/process.h>

uint64_t ticks = 0;
registers_t timer_callback(registers_t regs) {
	ticks++;
	return *schedule(&regs);
}
void init_pit() {
	register_interrupt_handler(IRQ0, &timer_callback);

	int divisor = 1193182 / FREQUENCY;

	outb(0x43, 0x36);

	// Divisor has to be sent in bytes
	uint8_t l = (uint8_t) (divisor & 0xFF);
	uint8_t h = (uint8_t) ((divisor >> 8) & 0xFF);

	outb(0x40, l);
	outb(0x40, h);
}
