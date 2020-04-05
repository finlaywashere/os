#include <arch/x86_64/tty.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_colour;
static uint16_t *terminal_buffer;

void terminal_initialize() {
	terminal_row = 0;
	terminal_column = 0;
	terminal_colour = VGA_COLOUR_LIGHT_GREY | VGA_COLOUR_BLACK << 4;
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = ((uint16_t) ' ') | terminal_colour << 8;
		}
	}
}
void terminal_setcolour(uint8_t colour) {
	terminal_colour = colour;
}
void terminal_putentryat(unsigned char c, uint8_t colour, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = ((uint16_t) c) | terminal_colour << 8;
}
void scroll(size_t amount) {
	for (size_t y = amount; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			const size_t newIndex = (y - amount) * VGA_WIDTH + x;
			terminal_buffer[newIndex] = terminal_buffer[index];
		}
	}
	terminal_row -= amount;
}
void terminal_putchar(char c) {
	unsigned char uc = c;
	if (uc == '\n') {
		terminal_row = terminal_row + 1;
		terminal_column = 0;
		return;
	}
	if (terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		terminal_row++;
	}
	if (terminal_row == VGA_HEIGHT) {
		scroll(1);
	}
	terminal_putentryat(uc, terminal_colour, terminal_column, terminal_row);
	terminal_column++;
}
void terminal_write(const char *data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
void terminal_writestring(const char *data) {
	terminal_write(data, strlen(data));
}
