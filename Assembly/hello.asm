; Print hello world

global main
extern puts

section .txt

main:
	mov rdi, message
	call puts
	ret

message: db "Hello World",0
	
