M_SRC:=$(wildcard kernel/*.c) 
SRC+=$(M_SRC)

eos.x86_64.elf: .EXTRA_PREREQS = libs/libk.a kernel/link.ld
eos.x86_64.elf: LDLIBS+=-lk
eos.x86_64.elf: $(M_SRC:.c=.o) kernel/ter-b.o kernel/ter-n.o kernel/dt.o
	$(LD) $(LDFLAGS) -T kernel/link.ld $^ -o $@ $(LDLIBS)

all: eos.x86_64.elf
