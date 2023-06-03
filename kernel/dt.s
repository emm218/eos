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
	retq
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
