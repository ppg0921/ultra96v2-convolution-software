#ifndef AXI_CONV_H
#define AXI_CONV_H

#include <stdint.h>

#define CONV_BASE_ADDR      0xA0000000
#define INPUT1_ADDR_OFFSET   0x00
#define INPUT2_ADDR_OFFSET   0x240 
#define WEIGHT_ADDR_OFFSET  0x120 //  64 x 3 x 3  = 576 inputs , 1 word = 8 input => 72 words = 288 bytes
#define CLEAR_ADDR_OFFSET   0xFFF4
#define START_ADDR_OFFSET   0xFFF8
#define DONE_ADDR_OFFSET    0xFFFC
#define RESULT_ADDR_OFFSET  0x0200  // start of result region

#define NUM_UNITS 112
#define INPUT_SIZE_PER_UNIT 9

#define IN_CHANNELS 64
#define OUT_CHANNELS 64 
#define OUTPUT_SIZE 112*112
#define INPUT_SIZE (112+2)*(112+2)

// Macro for 32-bit word access
#define CONV_WORD(offset) (*(volatile uint32_t *)(CONV_BASE_ADDR + (offset)))

// Function declarations
void conv_set_weights(const uint8_t *weights, uint32_t num_weights);
void conv_set_inputs(const uint8_t *inputs, uint32_t num_inputs);
void conv_start();
void conv_clear();
void conv_wait_done();
uint32_t conv_get_result(uint32_t unit_index);
void conv_test_multiple_units();

#endif // AXI_CONV_H
