CC=clang 
LD=ld.lld
AS=llvm-mc 
AR=ar

CFLAGS+=-target x86_64-none-elf -Wall -Wextra -Werror -ffreestanding -fpic \
				-mno-red-zone
ASFLAGS+=--arch=x86-64 --filetype=obj
LDFLAGS+=-L ./libs
LDLIBS:=-nostdlib

MODULES:=kernel libk

SRC:=

debug: CFLAGS+=-g -DDEBUG
debug: ASFLAGS+=-g
debug: LDFLAGS+=-g
debug: all

release: CFLAGS+=-O2
release: all

include $(patsubst %, %/.module.mk,$(MODULES))

OBJ:=$(SRC .c=.o)

img: eos.img

eos.img: eos.x86_64.elf eos.json cfg
	@mkdir -p boot/sys
	cp eos.x86_64.elf boot/sys/eos.x86_64.elf
	cp cfg boot/sys/cfg
	mkbootimg eos.json eos.img

%.o: %.psf
	cd $(dir $^) && \
	objcopy -I binary -O elf64-x86-64 \
		--rename-section .data=.rodata,alloc,load,readonly,data,contents $(notdir $^) $(notdir $@)

clean:
	rm -rf */*.o *.elf libs *.img boot

.depend/%.d: %.c
	@mkdir -p $(dir $@) 
	$(CC) $(CFLAGS) -MM $^ -MF $@

include $(patsubst %.c, .depend/%.d, $(filter %.c, $(SRC)))

.PHONY: release clean
