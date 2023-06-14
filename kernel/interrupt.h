#include <stdint.h>

struct interrupt_frame;

typedef struct {
	uint16_t isr_low;
	uint16_t cs;
	uint8_t ist;
	uint8_t attributes;
	unsigned long isr_high : 48;
	uint32_t reserved;
} __attribute__((packed)) idt_entry_t;

void default_interrupt_handler(struct interrupt_frame *);

void idt_init();
void set_isr(uint8_t, void *, uint8_t);
