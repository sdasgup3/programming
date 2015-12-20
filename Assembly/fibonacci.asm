; A 64-bit Linux application that writes the first 90 Fibonacci numbers.


global main
extern printf

section .txt

main:
	xor 	rax, 	rax	; rax = 0
	xor	rbx,	rbx
	inc 	rbx		; rbx = 1
	mov 	rcx,	90	; rcx = 90
	
	
print:
	push	rax
	push	rbx
	push	rcx

	mov	rdi, 	format
	mov	rsi, 	rbx
	xor     rax, rax                ; because printf is varargs
	call printf

	pop	rcx
	pop	rbx
	pop	rax

	mov	rdx, 	rbx
	add	rbx, 	rax
	mov	rax, 	rdx

	dec	rcx

	jnz	print

	ret			; Pop into the instruction pointer

format:
        db  "%20ld", 10, 0

