bits 64
global jmp_usermode
jmp_usermode:
	mov ax, 0x23
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov rax, rsp
	push 0x23
	push rax
	pushf
	push 0x1b
	push 0x0 ; entry point for user programs
	iret
