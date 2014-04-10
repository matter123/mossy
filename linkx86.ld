OUTPUT_FORMAT("elf32-i386")
ENTRY(start)

SECTIONS
{
	. = 0x100000;

	.setup :
	{
		*(.setup)
	}

	. += 0xC0000000;
	k_start = .;
	.text : AT(ADDR(.text) - 0xC0000000)
	{
		*(.mboot)
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
