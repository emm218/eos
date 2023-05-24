#include <stdint.h>

#include "bootboot.h"
#include "kprint.h"

extern BOOTBOOT bootboot;
extern unsigned char environment[4096];
extern uint8_t fb;

void
_start()
{
	kprintf("Hello, World!\n");
	kprintf("%+d %d %08x %p\n", 137, -18, 127, &fb);

	while (1)
		;
}
