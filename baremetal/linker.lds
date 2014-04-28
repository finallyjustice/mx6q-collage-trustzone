OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
	. = 0x0090A000;
	LOAD_ADR = . ;
	
	. = ALIGN(4);
	.flash_header : { *(.ivt) }
	
	. = ALIGN(4);
	.text : { *(.text) }

	. = ALIGN(4);
	.data : { *(.data) }
	
	. = ALIGN(4);
	END_ADR = . ;
	
	. = ALIGN(4);
	__bss_start = .;
	.bss : { *(.bss) }
	__bss_end = .;
	
	. = 0x00909000 ;
	.stack :
	{ 
	STACK_ADR = . ;
	}
}