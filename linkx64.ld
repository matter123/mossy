OUTPUT_FORMAT("elf64-x86-64")
ENTRY(start)
KERNEL_LMA = 0x100000;
KERNEL_VMA = 0xFFFFFFFF80000000;
SECTIONS
{
	. = KERNEL_LMA;
	.low_text :
	{
		*(.MBOOT)
		*(.low_text)
	}
	.low_bss :
	{
		*(.low_bss)
	}
	. += KERNEL_VMA;
	k_start = .;
	.text ALIGN(4096) : AT(ADDR(.text) - KERNEL_VMA)
	{
		*(.text)
	}
	.ctors ALIGN(4096) :AT(ADDR(.ctors) - KERNEL_VMA)
	{
		__CTOR_LIST__ = .;
		*(SORT(.ctors))
		QUAD(0)
	}
	.data ALIGN(4096) : AT(ADDR(.data) - KERNEL_VMA)
	{
		*(.data)
		*(.rodata*)
	}
	.bss ALIGN (4096) : AT(ADDR(.bss) - KERNEL_VMA)
	{
		*(COMMON*)
		*(.bss*)
		k_end = ALIGN(4096);
		. += 128K;
		k_data_end = .;
		. += 96K;
		k_stack_end = .;
		. += 4K;
	}
	/DISCARD/ :
	{
		*(.comment)
		*(.dtors)
	}
}