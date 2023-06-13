ENTRY(_start)
mmio				= 0xfffffffff8000000;
fb					= 0xfffffffffc000000;
bootboot		= 0xffffffffffe00000;
enviornment	= 0xffffffffffe01000;

PHDRS
{
	boot PT_LOAD;
}
SECTIONS
{
    . = 0xffffffffffe02000;
    __estart = .;
    .text : ALIGN(4096) {                       /* code */
        KEEP(*(.text.boot)) *(.text .text.*)    
    } :boot
    .data : ALIGN(4096) {                       /* data */
        *(.data .data.*)
    }
    .ro_data : ALIGN(4096) {                    /* read-only data */
        *(.rodata .rodata.*)
    }
    .bss (NOLOAD) : ALIGN(4096) {               /* bss */
        . = ALIGN(16);
        *(.bss .bss.*)
        *(COMMON)
    } :boot
    
    . = ALIGN(4096);
    __kernel_brk = .;
    /DISCARD/ : { *(.eh_frame) *(.comment) }
}

