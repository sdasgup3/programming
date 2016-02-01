RAX 
RBX
RCX
RDX
RBP
RSI
RDI
RSP

Linux system calls are called in exactly the same way as DOS system calls:
1.You put the system call number in EAX (we are dealing with 32-bit registers here)
You set up the arguments to the system call in EBX, ECX, etc.
You call the relevant interrupt (for DOS, 21h; for Linux, 80h)
The result is usually returned in EAX
