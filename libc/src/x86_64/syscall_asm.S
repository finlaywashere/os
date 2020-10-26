bits 64
global syscall_asm
syscall_asm:
	push rbx
	mov rax, rdi
	mov rbx, rsi
	mov r8, rcx
	mov rcx, rdx
	mov rdx, r8
	int 80
	pop rbx
	ret
