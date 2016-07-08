#include <hal/hal.h>

namespace hal {
void magic_break() { asm("xchg %bx, %bx"); }
void idle() { asm("hlt"); }
void halt() {
	disable_interrupts();
	idle();
}
void enable_interrupts() { asm("sti"); }
void disable_interrupts() { asm("cli"); }
}
