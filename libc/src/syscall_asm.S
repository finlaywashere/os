bits 64
global screen_putchar
screen_putchar:
	push rax
	push rbx
	
	mov rbx, rdi
	mov rax, 0
	int 80
	
	pop rbx
	pop rax
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
