#include <arch/x86_64/keyboard.h>
#include <arch/x86_64/io.h>
#include <arch/x86_64/isr.h>
#include <arch/x86_64/tty.h>
#include <kernel/string.h>

#define COMMAND_PORT 0x64
#define DATA_PORT 0x60

uint8_t caps = 0;
uint8_t caps_lock = 0;
uint8_t shift = 0;
uint8_t ctrl = 0;
uint8_t alt = 0;

uint16_t buffer[32];

uint16_t getKeyFromCode(uint8_t key1, uint8_t key2) {
	if (key1 != 0xE0) {
		if (key1 < 0x80) {
			return scancodeToKey1[key1];
		} else {
			return scancodeToKey1[key1 - 0x80] | RELEASED_BIT;
		}
	}else{
		if(key2 < 0x99){
			key2 -= 15;
			return specialScancodes1[key2];
		}else{
			key2 -= 0x99;
			return specialScancodes1[key2] | RELEASED_BIT;
		}
	}
}

void keyboard_callback(registers_t regs) {
	uint8_t key1 = inb(DATA_PORT);
	uint8_t key2 = inb(DATA_PORT);

	uint16_t key = getKeyFromCode(key1, key2);
	int released = key & RELEASED_BIT;
	if ((key >> 8) & SPECIAL_BITS) {
		uint8_t special_key = (key >> 8) & SPECIAL_BITS;
		if (special_key == LSHIFT) {
			if (released)
				shift = 0;
			else
				shift = 1;

		} else if (special_key == LCTRL) {
			if (released)
				ctrl = 0;
			else
				ctrl = 1;
		} else if (special_key == LALT) {
			if (released)
				alt = 0;
			else
				alt = 1;
		} else if (special_key == CAPS_LOCK) {
			if (released)
				caps_lock = 0;
			else
				caps_lock = 1;
		}
		caps = 0;
		if (caps_lock)
			caps = 1;
		if (shift)
			caps = 1;
		if (caps_lock && shift)
			caps = 0;
		return;
	}
	if (!released) {
		uint8_t asciiKey = key & 0xFF;
		if(asciiKey >= 'a' && asciiKey <=  'z' && caps){
			asciiKey -= 32;
			key &= 0xFF00;
			key |= asciiKey;
		}
		terminal_putchar(key);

		int found = 0;
		for (int i = 0; i < 32; i++) {
			if (!buffer[i]) {
				found = 1;
				buffer[i] = key;
				break;
			}
		}
		if (!found) {
			for (int i = 1; i < 32; i++) {
				buffer[i - 1] = buffer[i];
			}
			buffer[31] = key;
		}
	}
}

void init_keyboard() {
	register_interrupt_handler(IRQ1, &keyboard_callback);
	memset(&buffer, 0, sizeof(uint16_t) * 16);
}
