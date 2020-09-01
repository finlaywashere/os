BITS 64
mov rax, 0xffff8000000b8000
mov qword [rax], 0x4f524f45
loop:
	jmp loop
