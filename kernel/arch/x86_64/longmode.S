bits 64

section .bss
align 4096
; 64 bit GDT stuff
GDT642:                           ; Global Descriptor Table (64-bit).
    .Null: equ $ - GDT642         ; The null descriptor.
    dw 0xFFFF                    ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 0                         ; Access.
    db 1                         ; Granularity.
    db 0                         ; Base (high).
    .Code: equ $ - GDT642         ; The code descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10011010b                 ; Access (exec/read).
    db 10101111b                 ; Granularity, 64 bits flag, limit19:16.
    db 0                         ; Base (high).
    .Data: equ $ - GDT642         ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10010010b                 ; Access (read/write).
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
    .Pointer:                    ; The GDT-pointer.
    dw $ - GDT642 - 1             ; Limit.
    dq GDT642                     ; Base.

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
	lgdt [GDT642.Pointer]
	mov rsp, stack_top ; Sets rsp to the stack top which grows downwards
	;mov ax, 0
	;mov ss, ax
	;mov ds, ax
	;mov es, ax
	;mov fs, ax
	;mov gs, ax

	push rdi ; Push the multiboot header

	extern kernel_main
	call kernel_main

	; At this point something terrible has happened
	cli
.hang:	hlt
	jmp .hang
.end:
