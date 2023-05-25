.section .bss
.global gdtr
gdtr:
.skip 2 /* limit */
.skip 8 /* base */

.section .text
.global set_gdt
.type set_gdt, @function
set_gdt:
	subw 	$1, %si
	movw 	%si, gdtr 
	movq 	%rdi, gdtr + 2 
	lgdt	(gdtr) 
	retq
