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

