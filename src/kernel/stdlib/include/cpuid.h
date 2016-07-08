enum cpuid_requests {
	CPUID_GETVENDORSTRING,
	CPUID_GETFEATURES,
	CPUID_GETTLB,
	CPUID_GETSERIAL,

	CPUID_INTELEXTENDED = 0x80000000,
	CPUID_INTELFEATURES,
	CPUID_INTELBRANDSTRING,
	CPUID_INTELBRANDSTRINGMORE,
	CPUID_INTELBRANDSTRINGEND,
};

/** issue a single request to CPUID. Fits 'intel features', for instance
 *  note that even if only "eax" and "edx" are of interest, other registers
 *  will be modified by the operation, so we need to tell the compiler about it.
 */
__attribute__((unused)) static inline void cpuid(int code, uint32_t *a, uint32_t *d) {
	asm volatile("cpuid" : "=a"(*a), "=d"(*d) : "a"(code) : "ecx", "ebx");
}

/** issue a complete request, storing general registers output as a string
 */
__attribute__((unused)) static inline int cpuid_string(int code, uint32_t where[4]) {
	asm volatile("cpuid" : "=a"(*where), "=b"(*(where + 1)), "=c"(*(where + 2)), "=d"(*(where + 3)) : "a"(code));
	return (int)where[0];
}

__attribute__((unused)) static inline void outb(uint16_t port, uint8_t val) {
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}
__attribute__((unused)) static inline void outw(uint16_t port, uint16_t val) {
	asm volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}
__attribute__((unused)) static inline void outd(uint16_t port, uint32_t val) {
	asm volatile("outl %0, %1" : : "a"(val), "Nd"(port));
}
__attribute__((unused)) static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

__attribute__((unused)) static inline uint16_t inw(uint16_t port) {
	uint16_t ret;
	asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

__attribute__((unused)) static inline uint32_t ind(uint16_t port) {
	uint32_t ret;
	asm volatile("inl %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}
__attribute__((unused)) static inline void wrmsr(uint32_t msr, uint32_t lo, uint32_t hi) {
	asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}
__attribute__((unused)) static inline void rdmsr(uint32_t msr, uint32_t *lo, uint32_t *hi) {
	asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}
