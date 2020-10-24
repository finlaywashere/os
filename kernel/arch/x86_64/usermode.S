bits 64
extern irq0
global jump
jump:
	mov rsp, 0xffffffff801269a0
	mov rax, 0
	push rax
	push rax
	push rax
	push rax
	push rax
	sti
	jmp irq0
