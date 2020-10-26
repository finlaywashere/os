#include <libc/syscall.h>
int main(){
	print("\nTest1234");
	exec("test2.bin");
	print("\n5678");
	for(int i = 0; i < 5; i++){
		uint16_t character = readCharacter();
		char ascii = character & 0xFF;
		char buf[2];
		buf[0] = ascii;
		buf[1] = 0x0;
		print(&buf);
	}
	while(1){}
}

