bits 64
global call_cpuid
call_cpuid:
	mov rax, rdi
	cpuid
	mov r8, [rsi]
	mov [r8], rax
	mov [r8+0x8], rbx
	mov [r8+0x10], rcx
	mov [r8+0x18], rdx
	ret
