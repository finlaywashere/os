bits 64
global jump
jump:
	mov r8, rdi
	; Pop rdi twice to get the pointer outta the stack
	pop rdi
	pop rdi
        pop rsi
        pop rbp
        pop rbx
        pop rdx
        pop rcx
        pop rax
	pop rsp
	jmp r8

global jmp_usermode
jmp_usermode:
	mov r8, rdi
        ; Pop rdi twice to get the pointer outta the stack
        pop rdi
        pop rdi
        pop rsi
        pop rbp
        pop rbx
        pop rdx
        pop rcx
        pop rax
        pop rsp
        
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
	push rdi ; entry point for user programs
	iret
