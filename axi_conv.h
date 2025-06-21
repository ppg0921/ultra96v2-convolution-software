#ifndef AXI_CONV_H
#define AXI_CONV_H

#include <stdint.h>

#define CONV_BASE_ADDR      0xA0000000  // adjust this to match your block design
#define WEIGHT_ADDR_OFFSET  0x00
#define INPUT_ADDR_OFFSET   0x08
#define START_ADDR_OFFSET   0xFFF8  // (2^16 - 8)
#define RESULT_ADDR_OFFSET  0xFFF4  // (2^16 - 12)
#define DONE_ADDR_OFFSET    0xFFFC  // (2^16 - 4)

// Access macros
#define CONV_WORD(addr) (*(volatile uint32_t *)(CONV_BASE_ADDR + (addr)))

// Function declarations
void conv_set_weights(const uint8_t *weights, uint32_t num_weights);
void conv_set_inputs(const uint8_t *inputs, uint32_t num_inputs);
void conv_start();
void conv_wait_done();
uint32_t conv_get_result();

void conv_test();  // test 3x3 weights/inputs = 1~9

#endif // AXI_CONV_H
