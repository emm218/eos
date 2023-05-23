CC=clang -target i686-none-elf
LD=ld.lld
AS=llvm-mc --arch=x86 --filetype=obj

CFLAGS+=-Wall -Wextra -Werror -ffreestanding 
LDFLAGS+=-T link.ld -nostdlib

debug: CFLAGS+=-g
debug: eos.bin

release: CFLAGS+=-O2
release: eos.bin

eos.bin: kernel.o boot.o
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o *.iso *.bin

.PHONY: release clean
