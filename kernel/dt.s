/*
 * dt.s - 
 * 		assembly routines for setting global descriptor table and interrupt
 *		descriptor table
 */
.section .bss
.global gdtr
gdtr:
.skip 2 /* limit */
.skip 8 /* base */
.global idtr
idtr:
.skip 2 /* limit */
.skip 8 /* base */

.section .text
.global set_gdt
.type set_gdt, @function
set_gdt:
	cli
	subw 	$1, %si
	movw 	%si, gdtr 
	movq 	%rdi, gdtr + 2 
	lgdt	(gdtr) 
	sti
	/* mov $0x40, %ax
	ltr %ax */
	/* reload data segment registers */
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	popq %rdi
	movq $0x08, %rax
	pushq %rax
	pushq %rdi
	lretq
.global set_idt
.type set_idt, @function
set_idt:
	cli
	subw 	$1, %si
	movw 	%si, idtr 
	movq 	%rdi, idtr + 2 
	lidt	(idtr) 
	sti
	retq
