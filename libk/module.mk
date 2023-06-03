SRC+=$(wildcard libk/*.c)

libs/libk.a: libk/string.o
	ar rcs $@ $?

all: libs/libk.a
