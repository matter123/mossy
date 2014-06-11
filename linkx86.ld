OUTPUT_FORMAT("elf32-i386")
ENTRY(start)
KERNEL_LMA = 0x100000;
KERNEL_VMA = 0xC0000000;
SECTIONS
{
	. = KERNEL_LMA;
	.low_text ALIGN (4096) :
	{
		*(.mboot)
		*(.low_text)
	}
	.low_data ALIGN (4096) :
	{
		*(.low_data)
	}
	.low_bss ALIGN (4096) :
	{
		*(.low_bss)
	}
	. += KERNEL_VMA;

	k_start = .;
	.text : AT(ADDR(.text) - 0xC0000000)
	{
		*(.text)
	}

	.data ALIGN (4096) : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
		*(.rodata*)
	}

	.bss ALIGN (4096) : AT(ADDR(.bss) - 0xC0000000)
	{
		*(COMMON*)
		*(.bss*)
		k_end = .;
		. += 131072;
		k_data_end = .;
	}
}
