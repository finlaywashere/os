#include <libc/syscall.h>
int main(){
	print("\nTest1234");
	exec("test2.bin");
	print("\n5678");
	while(1){}
}

