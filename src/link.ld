OUTPUT_FORMAT("elf64-x86-64")
ENTRY(start)
kernel_lma = 0x100000;
kernel_vma = 0xFFFFFFFF80000000;
SECTIONS
{
	. = kernel_lma;
	.low_text :
	{
		*(.MBOOT)
		*(.low_text)
	}
	.low_bss :
	{
		*(.low_bss)
	}
	. += kernel_vma;
	k_start = .;
	.text ALIGN(4096) : AT(ADDR(.text) - kernel_vma)
	{
		*(.text)
	}
	.ctors ALIGN(4096) :AT(ADDR(.ctors) - kernel_vma)
	{
		__CTOR_LIST__ = .;
		*(SORT(.ctors))
		QUAD(0)
	}
	.data ALIGN(4096) : AT(ADDR(.data) - kernel_vma)
	{
		*(.data)
	}
	.rodata ALIGN(4096) : AT(ADDR(.rodata) - kernel_vma)
	{
		*(.rodata*)
	}
	.bss ALIGN (4096) : AT(ADDR(.bss) - kernel_vma)
	{
		*(COMMON*)
		*(.bss*)
		k_end = ALIGN(4096);
		. += 128K;
		k_data_end = .;
		. += 96K;
		k_stack_end = .;
		. += 32K;
	}
	/DISCARD/ :
	{
		*(.comment)
		*(.dtors)
	}
}
