bits 64
global write
write:
	push rax
	push rbx
	push rcx
	push rdx
	
	mov rbx, rdi
	mov rax, 0
	int 80
	
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

