section .data
	hello     db	'Hello, world!',10	; Our dear string
	helloLen  equ	$ - hello		; Length of our dear string

section	.text
    global main

main:
	pop	rbx		; argc (argument count)
	pop	rbx		; argv[0] (argument 0, the program name)
	pop	rbx		; The first real arg, a filename

	mov	rax,8		; The syscall number for creat() (we already have the filename in rbx)
	mov	rcx,00644Q	; Read/write permissions in octal (rw_rw_rw_)
	int	80h		; Call the kernel
				; Now we have a file descriptor in rax

	test	rax,rax		; Lets make sure the file descriptor is valid
	js	skipWrite	; If the file descriptor has the sign flag
				;  (which means it's less than 0) there was an oops,
				;  so skip the writing. Otherwise call the filewrite "procedure"
	call	fileWrite

skipWrite:
	mov	rbx,rax		; If there was an error, save the errno in rbx
	mov	rax,1		; Put the exit syscall number in rax
	int	80h		; Bail out

; proc fileWrite - write a string to a file
fileWrite:
	mov	rbx,rax		; sys_creat returned file descriptor into rax, now move into rbx
	mov	rax,4		; sys_write
				; rbx is already set up
	mov	rcx,hello	; We are putting the ADDRESS of hello in rcx
	mov	rdx,helloLen	; This is the VALUE of helloLen because it's a constant (defined with equ)
	int	80h

	mov	rax,6		; sys_close (rbx already contains file descriptor)
	int	80h
	ret
; endp fileWrite
