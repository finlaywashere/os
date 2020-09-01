BITS 64
;mov rax, 0xffff8000000b8000
;mov qword [rax], 0x4f524f45
mov rax, 'A'
int 80
mov rax, 'B'
int 80
mov rax, 'C'
int 80
loop:
	jmp loop
