# Register Usage
EAX 
EBX
ECX
EDX
ESI
EDI
EBP
ESP

## Linux system calls are called in exactly the same way as DOS system calls:
1.You put the system call number in EAX (we are dealing with 32-bit registers here)
2.You set up the arguments to the system call in EBX, ECX, etc.
  * EBX, ECX, EDX, ESI, EDI, and finally EBP
3.You call the relevant interrupt (for DOS, 21h; for Linux, 80h)
4.The result is usually returned in EAX
