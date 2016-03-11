
volatile uint16_t *vga_mem=reinterpret_cast<uint16_t *>(0xB8000);
uint16_t index=0;

void putc(char c) {
	vga_mem[index++]=15<<8|c;
}
