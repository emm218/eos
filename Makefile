CC=clang -target x86_64-none-elf
LD=ld.lld
AS=llvm-mc --arch=x86_64 --filetype=obj
AR=ar

CFLAGS+=-Wall -Wextra -Werror -ffreestanding -fpic 
LDFLAGS+=-L .
LDLIBS=-nostdlib -lk

debug: CFLAGS+=-g
debug: eos.x86_64.elf

release: CFLAGS+=-O2
release: eos.x86_64.elf

img: eos.img

eos.img: eos.x86_64.elf eos.json cfg
	mkdir -p boot/sys
	cp eos.x86_64.elf boot/sys/eos.x86_64.elf
	cp cfg boot/sys/cfg
	mkbootimg eos.json eos.img

eos.x86_64.elf: kernel.o kprint.o console.o terminus.o libk.a
	$(LD) $(LDFLAGS) -T link.ld $^ -o $@ $(LDLIBS)

libk.a: libk/string.o
	ar rcs libk.a $^

%.o: %.psf
	objcopy -I binary -O elf64-x86-64 \
		--rename-section .data=.rodata,alloc,load,readonly,data,contents $^ $@

clean:
	rm -rf *.o *.a *.elf *.img boot

.PHONY: release clean
