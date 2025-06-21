#ifndef AXI_DDR_H
#define AXI_DDR_H

#include <stdint.h>

// Base address of DDR (check Address Editor / platform config)
#define DDR_BASE_ADDR 0x00000000  // Default on Zynq UltraScale+

// Access macro: returns a 32-bit word pointer to a DDR offset
#define DDR_WORD(addr) (*(volatile uint32_t *)(DDR_BASE_ADDR + (addr)))

// Function declarations
void ddr_write(uint32_t addr, uint32_t data);
uint32_t ddr_read(uint32_t addr);
void ddr_test();

#endif // AXI_DDR_H
