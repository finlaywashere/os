bits 64
%macro pushAll 0
	push rsp
	push rax
	push rcx
	push rdx
	push rbx
	push rbp
	push rsi
	push rdi
%endmacro
%macro popAll 0
	pop rdi
	pop rsi
	pop rbp
	pop rbx
	pop rdx
	pop rcx
	pop rax
%endmacro

global enableInterrupts
enableInterrupts:
	sti
	ret
global disableInterrupts
disableInterrupts:
	cli
	ret

global load_idt
extern idt_ptr;
load_idt:
	lidt [idt_ptr]
	call enableInterrupts
	ret
global load_gdt
extern gdt_ptr;
load_gdt:
	cli
	lgdt [gdt_ptr]
	ret
global load_tss
load_tss:
	mov ax, 0x28
	ltr ax
	ret
extern isr_handler
isr_common:
	pushAll
	mov rbx, cr2
	push rbx
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
	pop rbx
	
	popAll
	pop r8
	add rsp, 16
	sti
	;call enableInterrupts
	iretq
%macro ISR_NOERRCODE 1
global isr%1
isr%1:
	;call disableInterrupts
	push byte 0
	push byte %1
	jmp isr_common
%endmacro
%macro ISR_ERRCODE 1
global isr%1
isr%1:
	;call disableInterrupts
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

extern irq_handler

irq_common_stub:
	pushAll
        mov rbx, cr2
        push rbx
        mov ax, ds
        push rax
        mov ax, 0x18
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs,ax

        call irq_handler

        pop rax
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        pop rbx

        popAll
	pop r8
        add rsp, 16
	;sti
        iretq


%macro IRQ 2
global irq%1
irq%1:
	;cli
	push byte 0
	push byte %2
	jmp irq_common_stub
%endmacro
IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

IRQ 80, 80
