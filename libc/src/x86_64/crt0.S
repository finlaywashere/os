section .text

extern main
extern initialize_standard_library
extern _init
extern exit

global _start
_start:
	; Set up end of the stack frame linked list.
	mov rbp, 0
	push rbp ; rip=0
	push rbp ; rbp=0
	mov rbp, rsp

	; We need those in a moment when we call main.
	push rsi
	push rdi

	; Prepare signals, memory allocation, stdio and such.
	; call initialize_standard_library

	; Run the global constructors.
	; Time to uncomment this later when it doesn't break stuff
	; call _init

	; Restore argc and argv.
	pop rdi
	pop rsi

	; Run main
	call main

	; Terminate the process with the exit code.
	mov edi, eax
	;call exit
