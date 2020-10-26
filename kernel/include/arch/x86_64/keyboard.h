#include <stdint.h>

void init_keyboard();

uint16_t scancodeToKey1[] = {
		0x0,0x1B,'1',
		'2','3','4',
		'5','6','7',
		'8','9','0',
		'-','=',0x8,
		'\t','q','w',
		'e','r','t',
		'y','u','i',
		'o','p','[',
		']','\n',15<<8,
		'a','s','d',
		'f','g','h',
		'j','k','l',
		';','\'','`',
		16<<8,'\\',
		'z','x','c',
		'v','b','n',
		'm',',','.',
		'/',17<<8,'*'|(18<<8),
		19<<8,' ',20<<8,1<<8,
		2<<8,3<<8,4<<8,
		5<<8,6<<8,7<<8,
		8<<8,9<<8,10<<8,
		21<<8,22<<8,'7'|(18<<8),
		'8'|(18<<8),'9'|(18<<8),'-'|(18<<8),
		'4'|(18<<8),'5'|(18<<8),'6'|(18<<8),
		'+'|(18<<8),'1'|(18<<8),'2'|(18<<8),
		'3'|(18<<8),'0'|(18<<8),'.'|(18<<8),
		0x0,0x0,0x0,
		11<<8,12<<8
};
uint16_t specialScancodes1[] = {
		0x0,23<<8,0x0,
		0x0,0x0,0x0,
		24<<8,0x0,0x0,
		'\n'|(18<<8),25<<8,0x0,
		0x0,26<<8,27<<8,
		28<<8,0x0,29<<8,
		0x0,0x0,0x0,
		0x0,0x0,30<<8,
		0x0,31<<8,0x0,
		32<<8,0x0,0x0,
		'/'|(18<<8),0x0,0x0,
		33<<8,0x0,0x0,
		0x0,0x0,0x0,
		0x0,34<<8,35<<8,
		39<<8,0x0,37<<8,
		0x0,38<<8,0x0,
		41<<8,36<<8,40<<8,
		42<<8,0x7F
};

#define F12 12
#define F11 11
#define F10 10
#define F9 9
#define F8 8
#define F7 7
#define F6 6
#define F5 5
#define F4 4
#define F3 3
#define F2 2
#define F1 1
#define ESCAPE 0x1B
#define BACKSPACE 0x8
#define LCTRL 15
#define RCTRL 25
#define LSHIFT 16
#define RSHIFT 17
#define LALT 19
#define RALT 33
#define CAPS_LOCK 20
#define NUM_LOCK 21
#define SCROLL_LOCK 22
#define HOME 34
#define END 41
#define INSERT 42

#define ARROW_UP 35
#define ARROW_DOWN 36
#define ARROW_LEFT 37
#define ARROW_RIGHT 38

#define PAGE_UP 39
#define PAGE_DOWN 40

#define NUMPAD_BIT 18
#define RELEASED_BIT 1<<15
#define SPECIAL_BITS 0b01111111

#define MUME_PT 23
#define MUME_NT 24
#define MUME_MU 26
#define MUME_CA 27
#define MUME_PL 28
#define MUME_ST 29
#define MUME_VD 30
#define MUME_VU 31
#define MUME_WWW_HOME 32

uint16_t getCurrKey();
uint16_t nextKey();
uint8_t toASCII(uint16_t);
