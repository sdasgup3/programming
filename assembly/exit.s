.section .text
.globl _start
_start:
    movl	$1, %eax	# tell kernel use system call #1 for exit
    movl	$0xff, %ebx	# exit with status 255
    int		$0x80		# interrupt 80 is do system call in %eax
