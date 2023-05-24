# EOS

Ἠώς is the dawn

## Toolchain

The makefile is set up to use all LLVM tools since they're vastly easier to do
cross compilation with than gcc. Compiling with GNU instead should be as simple
as changing the CC, AS, and LD variables in the makefile to an appropriate
cross-compilation toolchain.

## Bootloader

This project uses [BOOTBOOT](https://gitlab.com/bztsrc/bootboot/) since I didn't
want to deal with the details of enabling long mode and doing higher half
mapping myself (maybe in the future I'll try to make it GRUB compatible again).
Because of this, the `img` target in the makefile depends on BOOTBOOT's
`mkbootimg` tool.

## License

All code is MIT licensed. The [Terminus](https://terminus-font.sourceforge.net/)
font used as a console font is licensed under the OFL, a copy of which is
included with this repository.
