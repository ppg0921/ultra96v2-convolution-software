#ifndef AXI_RAM_H
#define AXI_RAM_H

#include <stdint.h>

#define RAM_BASE_ADDR 0x00A0000000

// RAM access macros
#define RAM_WORD(addr) (*(volatile uint32_t *)(RAM_BASE_ADDR + (addr)))

// Function declarations
void ram_write(uint32_t addr, uint32_t data);
uint32_t ram_read(uint32_t addr);
void ram_test();

#endif // AXI_RAM_H
