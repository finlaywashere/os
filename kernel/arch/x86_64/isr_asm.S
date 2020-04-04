%macro pushAll 0
	push rax
	push rcx
	push rdx
	push rbx
	push rsp
	push rbp
	push rsi
	push rdi
%endmacro
%macro popAll 0
	pop rdi
	pop rsi
	pop rbp
	pop rsp
	pop rbx
	pop rdx
	pop rcx
	pop rax
%endmacro

global load_idt
extern idt_ptr;
load_idt:
	lidt [idt_ptr]
	ret
extern isr_handler
isr_common:
	pushAll
	mov ax, ds
	push rax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs,ax

	call isr_handler

	pop rax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popAll
	add rsp, 16
	sti
	iretq
%macro ISR_NOERRCODE 1
global isr%1
isr%1:
	cli
	push byte 0
	push byte %1
	jmp isr_common
%endmacro
%macro ISR_ERRCODE 1
global isr%1
isr%1:
	cli
	push byte %1
	jmp isr_common
%endmacro
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31