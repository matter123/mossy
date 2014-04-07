OUTPUT_FORMAT("elf64-x86-64")
ENTRY(start)

VIRT_BASE = 0xFFFFFFFF80000000;

SECTIONS
{
	. = 0x100000;

	.boot :
	{
		*(.mboot)
		load_start = .;
		*(.boot)
		. = ALIGN(4096);
		pml4 = .;
		. += 0x1000;
		pdpt = .;
		. += 0x1000;
		pd = .;
		. += 0x1000;
		. += 0x8000;
		stack = .;
	}

	. += VIRT_BASE;

	.text ALIGN(0x1000) : AT(ADDR(.text) - VIRT_BASE)
	{
		*(.text)
	}

	.data ALIGN(0x1000) : AT(ADDR(.data) - VIRT_BASE)
	{
		*(.data)
	}

	.rodata ALIGN(0x1000) : AT(ADDR(.rodata) - VIRT_BASE)
	{
		*(.rodata*)
	}

	load_end = . - VIRT_BASE;

	.bss ALIGN(0x1000) : AT(ADDR(.bss) - VIRT_BASE)
	{
		k_end = .;
		*(COMMON)
		*(.bss)
	}

	bss_end = . - VIRT_BASE;

	.jcr ALIGN(0x1000) : AT(ADDR(.jcr) - VIRT_BASE)
	{
		*(.jcr)
	}
	.ctors ALIGN(0x1000) : AT(ADDR(.ctors) - VIRT_BASE)
	{
		*(.ctors)
	}
	.dtors ALIGN(0x1000) : AT(ADDR(.dtors) - VIRT_BASE)
	{
		*(.dtors)
	}
	.eh_frame ALIGN(0x1000) : AT(ADDR(.eh_frame) - VIRT_BASE)
	{
		*(.eh_frame)
	}
	.kdata ALIGN (0x1000) : AT(ADDR(.kdata) - VIRT_BASE)
	{
		*(.kdata)
		. += 131072;
		k_data_end = .;
	}

	/DISCARD/ :
	{
		*(.comment)
	}
}