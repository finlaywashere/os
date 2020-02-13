MBALIGN		equ	1 << 0		; Align loaded modules on page boundaries
MEMINFO		equ	1 << 1		; Provide memory map
FLAGS		equ	MBALIGN | MEMINFO
MAGIC		equ	0x1BADB002
CHECKSUM	equ	-(MAGIC + FLAGS)

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; Sets up the stack for the kernel
section .bss
align 16
stack_bottom:
resb 16384 ; 16KiB
stack_top:

section .text
global _start:function (_start.end-_start)
_start:
	mov esp, stack_top ; Sets esp to the stack top which grows downwards
	
	push eax ; Push the multiboot header
	push ebx ; Push the multiboot magic
	
	extern kernel_main
	call kernel_main
	
	; At this point something terrible has happened
	cli
.hang:	hlt
	jmp .hang
.end:
