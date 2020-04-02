bits 64
global long_mode_start
long_mode_start:
	mov ax, 0
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov rax, 0x2f592f412f4b2f4f
	mov qword [0xb8000], rax


	push rdi ; Push the multiboot header

	extern kernel_main
	call kernel_main

	; At this point something terrible has happened
	cli
.hang:	hlt
	jmp .hang
.end:
