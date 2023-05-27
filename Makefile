CC=clang 
LD=ld.lld
AS=llvm-mc 
AR=ar

CFLAGS+=-target x86_64-none-elf -Wall -Wextra -Werror -ffreestanding -fpic -mno-red-zone
ASFLAGS+=--arch=x86-64 --filetype=obj
LDFLAGS+=-L ./libs
LDLIBS=-nostdlib -lk

debug: CFLAGS+=-g
debug: ASFLAGS+=-g
debug: eos.x86_64.elf

release: CFLAGS+=-O2
release: eos.x86_64.elf

img: eos.img

eos.img: eos.x86_64.elf eos.json cfg
	mkdir -p boot/sys
	cp eos.x86_64.elf boot/sys/eos.x86_64.elf
	cp cfg boot/sys/cfg
	mkbootimg eos.json eos.img

eos.x86_64.elf: .EXTRA_PREREQS = libs/libk.a
eos.x86_64.elf: kernel.o paging.o interrupt.o dt.o kprint.o console.o terminus.o
	$(LD) $(LDFLAGS) -T link.ld $^ -o $@ $(LDLIBS)

libs/libk.a: libk/string.o
	ar rcs $@ $?

%.o: %.psf
	objcopy -I binary -O elf64-x86-64 \
		--rename-section .data=.rodata,alloc,load,readonly,data,contents $^ $@

clean:
	rm -rf *.o */*.o libs/* *.elf *.img boot

.PHONY: release clean
