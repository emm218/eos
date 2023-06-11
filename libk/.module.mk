SRC+=$(wildcard libk/*.c)

libs/libk.a: libk/string.o
	@mkdir -p libs
	ar rcs $@ $?

all: libs/libk.a
