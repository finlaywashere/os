bits 64
global screen_putchar
screen_putchar:
	mov rax, 0
	push rbx
	int 80
	ret
global screen_setpos
screen_setpos:
	mov rax, 1
	mov rbx, rbp
	int 80
	ret
global screen_getpos
screen_getpos:
	mov rax, 2
	int 80
	ret
