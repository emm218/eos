file eos.x86_64.elf
define connect
	target remote :1234
end
connect

break __panic
break paging_init
