#include <arch/lapic.h>
#include <arch/paging.h>
#include <acpi/acpi.h>
#include <cpuid.h>

bool check_apic_exists() {
	uint32_t eax, edx;
	cpuid(1, &eax, &edx);
	return edx & 1 << 9;
}

void initalize_lapic() {
	if(!check_apic_exists()) { Log(LOG_ERROR, "[LAPIC ]", "cpuid says lapic doesn't exist"); }
	// reset address
	uint32_t eax, edx;
	rdmsr(0x1B, &eax, &edx);
	wrmsr(0x1B, eax | 0x800, edx);
	ACPI_TABLE_MADT *madt = (ACPI_TABLE_MADT *)GetACPITable("APIC");
	if(madt->Flags) {
		// dual PIC exists do disable PIC by remapping and masking
		// remap PIC
		outb(0x20, 0x11);
		outb(0xA0, 0x11);
		outb(0x21, 0xF7);
		outb(0xA1, 0xF7);
		outb(0x21, 0x4);
		outb(0xA1, 0x2);
		outb(0x21, 0x1);
		outb(0xA1, 0x1);
		// mask PCI
		outb(0x21, 0xFF);
		outb(0xA1, 0xFF);
	}
	// remap lapic address
	Log(LOG_DEBUG, "[LAPIC ]", "PhysicalAddress: %p", madt->Address);
	map(madt->Address, 0xFFFFFEDFBFF00000, PAGE_WRITE);
	// enable lapic
	write_lapic_register(LAPIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER, 0x1FF);
	Log(LOG_DEBUG, "[LAPIC ]", "spurious interrupt vector is: %x",
	    read_lapic_register(LAPIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER));
	Log(LOG_INFO, "[LAPIC ]", "lapic initalized, id=%d", read_lapic_register(LAPIC_ID_REGISTER));
}

void write_lapic_register(int reg, uint32_t value) {
	uintptr_t addr = 0xFFFFFEDFBFF00000;
	addr |= reg;
	*reinterpret_cast<uint32_t *>(addr) = value;
}
uint32_t read_lapic_register(int reg) {
	uintptr_t addr = 0xFFFFFEDFBFF00000;
	addr |= reg;
	return *reinterpret_cast<uint32_t *>(addr);
}

void set_lapic_timer(int mode, int divisor, int vector, uint32_t count) {
	Log(LOG_DEBUG, "[LAPIC ]",
	    "setting timer in mode: %x, at vector: %d, with a divisor of: %d and inital count of: %x", mode, vector,
	    divisor, count);
	uint32_t mask = ~(0xFF | 7 << 16);
	uint32_t bitset = mode << 17 | vector;
	Log(LOG_DEBUG, "[LAPIC ]", "timer mask=%x and bitset=%x", mask, bitset);
	if(mode == LAPIC_TIMER_MODE_DISABLED) {
		write_lapic_register(LAPIC_LVT_TIMER_REGISTER,
		                     (read_lapic_register(LAPIC_LVT_TIMER_REGISTER) & mask) | 1 << 16);
	} else if(mode == LAPIC_TIMER_MODE_TSC_DEALINE) {
		// check is TSC deadline mode is supported
		// this is derived from the TSC-dealine enabling code on page 10-17 of volume 3  of the intel manual
		uint32_t regs[4];
		cpuid_string(CPUID_GETFEATURES, regs);
		if(!regs[2] & 1 << 24) { Log(LOG_WARNING, "[LAPIC ]", "this PC does not support TSC deadline mode"); }
		write_lapic_register(LAPIC_LVT_TIMER_REGISTER, (read_lapic_register(LAPIC_LVT_TIMER_REGISTER) & mask) | bitset);
		wrmsr(0x6E0, count, 0);
	} else {
		// write_lapic_register(LAPIC_INITAL_COUNT_REGISTER, 0);
		write_lapic_register(LAPIC_DIVIDE_CONFIGURATION_REGISTER,
		                     (read_lapic_register(LAPIC_DIVIDE_CONFIGURATION_REGISTER) & ~16) | divisor);
		uint32_t timer_register = read_lapic_register(LAPIC_LVT_TIMER_REGISTER);
		Log(LOG_DEBUG, "[LAPIC ]", "timer lvt: %x", timer_register);
		write_lapic_register(LAPIC_LVT_TIMER_REGISTER, (timer_register & mask) | bitset);
		write_lapic_register(LAPIC_INITAL_COUNT_REGISTER, count);
	}
}

void lapic_eoi() { write_lapic_register(LAPIC_EOI_REGISTER, 0); }
