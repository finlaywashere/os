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
bits 32
; Sets up the stack for the kernel
section .bss
align 16
align 4096
p4_table:
	resb 4096
p3_table:
	resb 4096
p2_table:
	resb 4096
p1_table:
	resb 4096

stack_bottom:
resb 1048576 ; 1MiB
stack_top:

section .rodata
GDT64:                           ; Global Descriptor Table (64-bit).
    .Null: equ $ - GDT64         ; The null descriptor.
    dw 0xFFFF                    ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 0                         ; Access.
    db 1                         ; Granularity.
    db 0                         ; Base (high).
    .Code: equ $ - GDT64         ; The code descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10011010b                 ; Access (exec/read).
    db 10101111b                 ; Granularity, 64 bits flag, limit19:16.
    db 0                         ; Base (high).
    .Data: equ $ - GDT64         ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10010010b                 ; Access (read/write).
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
    .Pointer:                    ; The GDT-pointer.
    dw $ - GDT64 - 1             ; Limit.
    dq GDT64                     ; Base.

section .text


; Prints `ERR: ` and the given error code to screen and hangs.
; parameter: error code (in ascii) in al
error:
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov dword [0xb8008], 0x4f204f20
    mov byte  [0xb800a], al
    hlt

check_multiboot:
    cmp eax, 0x2BADB002
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, "0"
	jmp error
check_cpuid:
	; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
	; in the FLAGS register. If we can flip it, CPUID is available.
	pushfd
	pop eax
	mov ecx, eax
	; Flip the ID bit
	xor eax, 1 << 21

	push eax
	popfd
	pushfd
	pop eax

	; Compare eax and ecx, if the bit is flipped then the cpu is 64 bit
	cmp eax, ecx
	je .no_cpuid
	ret
.no_cpuid:
	mov al, "1"
	jmp error
check_long_mode:
	; Test if long mode is available
	mov eax, 0x80000000 ; implicit argument for cpuid
	cpuid
	cmp eax, 0x80000001 ; needs to be atleast 0x80000001 for long mode
	jb .no_long_mode

	; use extended cpu info to test if long mode is available
	mov eax, 0x80000001
	cpuid ; returns cpu info
	test edx, 1 << 29 ; tests if the LM bit is set
	jz .no_long_mode ; if its not set then error
	ret
.no_long_mode:
	mov al, "2"
	jmp error
set_up_page_tables:
	; map first P4 entry to P3 table
	mov eax, p3_table
	or eax, 0b11 ; Present and writable
	mov [p4_table], eax

	; map first P3 entry to P2
	mov eax, p2_table
	or eax, 0b11 ; Present and writable
	mov [p3_table], eax

	mov ecx, 0 ; counter
.map_p2_table:
	; map ecx-th P2 entry to a huge page at address 2MiB*ecx
	mov eax, 0x200000 ; 2MiB
	mul ecx ; start address of page
	or eax,0b10000011 ; present, writable, and huge
	mov [p2_table + ecx*8], eax ; map entry

	inc ecx
	cmp ecx, 512 ; if the counter is 512 then the whole table is mapped
	jne .map_p2_table ; else map some more

	ret
enable_paging:
	; load P4 to the cr3 register
	mov eax, p4_table
	mov cr3, eax

	; enable PAE flag in cr4
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; set long mode bit
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	; enable paging in cr0
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax
	ret
global _start
_start:
	mov esp, stack_top ; Sets esp to the stack top which grows downwards
	
	mov edi,ebx ; Move multiboot header to edi

	call check_multiboot
	call check_cpuid
	call check_long_mode

	call set_up_page_tables
	call enable_paging

	lgdt [GDT64.Pointer]
	
	extern long_mode_start
	jmp GDT64.Code:long_mode_start

