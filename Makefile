CC=clang -target x86_64-none-elf
LD=ld.lld
AS=llvm-mc --arch=x86_64 --filetype=obj

CFLAGS+=-Wall -Wextra -Werror -ffreestanding -fpic 
LDLIBS=-nostdlib

debug: CFLAGS+=-g
debug: eos.x86_64.elf

release: CFLAGS+=-O2
release: eos.x86_64.elf

eos.x86_64.elf: kernel.o
	$(LD) $(LDFLAGS) -T link.ld $^ -o $@ $(LDLIBS)

clean:
	rm -rf *.o *.elf *.img boot

.PHONY: release clean
