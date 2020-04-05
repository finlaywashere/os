#include <arch/x86_64/keyboard.h>
#include <arch/x86_64/io.h>
#include <arch/x86_64/isr.h>
#include <kernel/timer.h>
#include <arch/x86_64/tty.h>
#include <kernel/kernel.h>

#define COMMAND_PORT 0x64
#define DATA_PORT 0x60

uint8_t caps = 0;
uint8_t caps_lock = 0;
uint8_t shift = 0;
uint8_t ctrl = 0;

void keyboard_callback(registers_t regs) {
	terminal_writestring("Received key press!\n");
	uint8_t key = inb(DATA_PORT);
}

void init_keyboard() {
	register_interrupt_handler(IRQ1, &keyboard_callback);
}
