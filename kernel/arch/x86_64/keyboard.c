#include <arch/x86_64/keyboard.h>
#include <arch/x86_64/io.h>
#include <arch/x86_64/isr.h>
#include <kernel/timer.h>
#include <arch/x86_64/tty.h>
#include <kernel/kernel.h>

#define COMMAND_PORT 0x64
#define DATA_PORT 0x60

void keyboardCallback(registers_t regs) {
	terminal_writestring("Received key press!");
}

void init_keyboard() {
	// Disable keyboard input
	outb(COMMAND_PORT, 0xAD);
	sleep(5);
	outb(COMMAND_PORT, 0xA7);
	sleep(5);
	// Clear data buffer
	inb(DATA_PORT);
	sleep(5);
	// Get config
	outb(COMMAND_PORT, 0x20);
	sleep(5);
	uint8_t configByte = inb(DATA_PORT);
	sleep(5);
	// Disable input and all that stuff
	configByte &= 0b10111100;
	outb(COMMAND_PORT, 0x60);
	sleep(5);
	outb(DATA_PORT, configByte);
	sleep(5);
	// Do a controller self test
	outb(COMMAND_PORT, 0xAA);
	sleep(5);
	uint8_t response = inb(DATA_PORT);
	if (response != 0x55)
		panic("Failed to initialize keyboard!\nSelf test failed\n");
	sleep(5);
	// Reload the config byte
	outb(COMMAND_PORT, 0x60);
	sleep(5);
	outb(DATA_PORT, configByte);
	sleep(5);
	// Test the controller interface
	outb(COMMAND_PORT,0xAB);
	sleep(5);
	response = inb(DATA_PORT);
	if(response)
		panic("Failed to initialize keyboard!\nInterface test failed\n");
	sleep(5);
	// Enable the first port
	outb(COMMAND_PORT,0xAE);
	sleep(5);
	// Send a new config that has interrupts
	outb(COMMAND_PORT,0x60);
	sleep(5);
	configByte |= 1;
	outb(DATA_PORT,configByte);

	register_interrupt_handler(IRQ1, &keyboardCallback);
}
