bits 64
section .data

msg:	db "Hello there kernel from syscall", 0 ; Null terminated string

section .bss
align 8

; Sets up the stack for the kernel
stack_bottom:
resb 0x1000 ; setup 4kb stack
stack_top:

section .text

_start:
	mov rsp, stack_top ; Sets rsp to the stack top which grows downwards

	mov rsi, msg
	mov rax, 0

	extern print
	call print

	pop rbp

loop:
	jmp loop
