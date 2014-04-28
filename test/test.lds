MEMORY
{
	RAM (rwx)  : ORIGIN = 0x10800000, LENGTH = 16K
}

SECTIONS
{
	. = ALIGN(4);
	.text :
	{
		start.o(*)
		*(.text)
	} > RAM

	. = ALIGN(4);
	.bss :
	{
		*(.bss)
	} > RAM

	. = ALIGN(4);
	.stack :
	{
		. += 0x100;
		__stack_end = .;
	} > RAM

	/DISCARD/ :
	{
		*(.comment)
		*(.ARM.attributes)
	}
}
