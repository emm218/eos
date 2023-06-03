#include <stdint.h>

struct interrupt_frame {
	uint64_t instruction_ptr;
	uint64_t code_segment;
	uint64_t rflags;
	uint64_t rstack_ptr;
	uint64_t stack_segment;
};

__attribute__((interrupt)) void
interrupt_handler(struct interrupt_frame *frame)
{
	(void)frame;
}
