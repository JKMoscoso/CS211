	.code32
	.globl nCr
        .type   nCr, @function
nCr:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ebx
	subl	$20, %esp
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	Factorial
	jo	.L7
	movl	%eax, %ebx
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	Factorial
	movl	%eax, %esi
	movl	12(%ebp), %eax
	movl	8(%ebp), %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, (%esp)
	call	Factorial
	imull	%eax, %esi
	movl	%esi, %ecx
	movl	%ebx, %eax
	cltd
	idivl	%ecx
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	addl	$20, %esp
	popl	%ebx
	popl	%esi
	popl	%ebp
	ret
.L7:
	movl 	$0, %eax
	leave
	ret

	.globl Factorial
        .type   Factorial, @function
Factorial:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$1, -4(%ebp)
	cmpl	$0, 8(%ebp)
	jns	.L2
	movl	$0, %eax
	jmp	.L3
.L2:
	movl	$1, -8(%ebp)
	jmp	.L4
.L6:
	movl 	$0, %eax
	leave
	ret
.L5:
	movl	-4(%ebp), %eax
	imull	-8(%ebp), %eax
	jo	.L6
	movl	%eax, -4(%ebp)
	addl	$1, -8(%ebp)
.L4:
	movl	-8(%ebp), %eax
	cmpl	8(%ebp), %eax
	jle	.L5
	movl	-4(%ebp), %eax
.L3:
	leave
	ret
