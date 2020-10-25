bits 64
global write
write:
	push rax
	push rbx
	push rcx
	push rdx
	
	mov rax, 0
	mov rdx, rdi
	int 80
	
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret
global exit_asm
exit_asm:
	mov rax, 1
	int 80
	ret
global exec_asm
exec_asm:
	mov rax, 2
	mov rbx, rdi
	int 80
	ret
