bits 64

section .bss
align 8

; Sets up the stack for the kernel
stack_bottom:
resb 0x10000 ; 1MiB
stack_top:

section .low_text
global long_mode_start
long_mode_start:
        mov rax, start_higher_half
        jmp rax


section .text
global start_higher_half
start_higher_half:
	mov rsp, stack_top ; Sets rsp to the stack top which grows downwards
	mov ax, 16
	mov ds, ax ; set data segment to kernel data
	mov ax, 0
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	push rdi ; Push the multiboot header

	extern kernel_main
	call kernel_main

	; At this point something terrible has happened
	cli
.hang:	hlt
	jmp .hang
.end:
