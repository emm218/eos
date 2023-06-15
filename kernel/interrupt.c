#include <stdint.h>

#include "dt.h"
#include "interrupt.h"
#include "kprint.h"
#include "paging.h"

struct interrupt_frame {
	uint64_t instruction_ptr;
	uint64_t code_segment;
	uint64_t rflags;
	uint64_t rstack_ptr;
	uint64_t stack_segment;
};

/* static void panic_todo(void); */

__attribute__((aligned(0x10))) idt_entry_t idt[256];

__attribute__((interrupt)) void
default_interrupt_handler(struct interrupt_frame *frame)
{
	(void)frame;
	asm volatile("cli\nhlt");
}

/*
static void
panic_todo(void)
{
	PANIC("todo!");
}
*/

void
set_isr(uint8_t vec, void *isr, uint8_t flags)
{
	idt_entry_t *cur = idt + vec;

	cur->isr_low = (uint64_t)isr & 0xFFFF;
	cur->cs = KERNEL_CS;
	cur->ist = 0;
	cur->attributes = flags;
	cur->isr_high = (uint64_t)isr >> 16;
	cur->reserved = 0;
}

void
idt_init()
{
	uint8_t i;
	asm volatile("cli");

	for (i = 0; i < 255; i++) {
		set_isr(i, default_interrupt_handler, 0x8E);
	}

	set_idt(va_to_pa(idt), sizeof(idt));

	asm volatile("sti");
}
