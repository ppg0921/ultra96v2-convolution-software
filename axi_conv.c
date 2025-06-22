#include "axi_conv.h"
#include "xil_printf.h"

void conv_set_weights(const uint8_t *weights, uint32_t num_weights) {
    uint32_t word = 0;
    for (uint32_t i = 0; i < num_weights; ++i) {
        word |= (weights[i] & 0xF) << (28 - 4 * (i % 8));
        if ((i % 8 == 7) || (i == num_weights - 1)) {
            uint32_t addr = WEIGHT_ADDR_OFFSET + 4 * (i / 8);  // Weights at 0xFC, 0xF8, ...
            CONV_WORD(addr) = word;
            xil_printf("Setting weight at addr 0x%08X: 0x%08X\n", addr, word);
            word = 0;
        }
    }
}

void conv_set_inputs(const uint8_t *inputs, uint32_t num_inputs) {
    uint32_t word = 0;
    for (uint32_t i = 0; i < num_inputs; ++i) {
        word |= (inputs[i] & 0xF) << (28 - 4 * (i % 8));
        if ((i % 8 == 7) || (i == num_inputs - 1)) {
            uint32_t addr = INPUT_ADDR_OFFSET + 4 * (i / 8);
            CONV_WORD(addr) = word;
            xil_printf("Setting input at addr 0x%08X: 0x%08X\n", addr, word);
            word = 0;
        }
    }
}

void conv_start() {
    CONV_WORD(START_ADDR_OFFSET) = 1;
}

void conv_wait_done() {
    while (CONV_WORD(DONE_ADDR_OFFSET) == 0);
}

uint32_t conv_get_result(uint32_t unit_index) {
    return CONV_WORD(RESULT_ADDR_OFFSET + 4 * unit_index);
}

void conv_test_multiple_units() {
    xil_printf("=== Multi-Unit Convolution Test ===\n");

    uint8_t weights[9];
    uint8_t inputs[NUM_UNITS * INPUT_SIZE_PER_UNIT];

    // Initialize weights = 1 ~ 9
    for (uint8_t i = 0; i < 9; ++i) {
        weights[i] = i + 1;
    }

    // Initialize inputs = 1 ~ (56 * 9)
    for (uint32_t i = 0; i < NUM_UNITS * INPUT_SIZE_PER_UNIT; ++i) {
        inputs[i] = (i + 1) % 16;
    }

    conv_set_weights(weights, 9);
    conv_set_inputs(inputs, NUM_UNITS * INPUT_SIZE_PER_UNIT);

    xil_printf("Weights and Inputs set. Starting convolution...\n");
    conv_start();
    conv_wait_done();

    xil_printf("Convolution done. Reading results:\n");

    for (uint32_t i = 0; i < NUM_UNITS; ++i) {
        uint32_t result = conv_get_result(i);
        xil_printf("Result[%2d] = 0x%08X\n", i, result);
    }
}
