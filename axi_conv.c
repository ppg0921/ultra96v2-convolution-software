#include "axi_conv.h"
#include "xil_printf.h"

void conv_set_weights(const uint8_t *weights, uint32_t num_weights) {
    uint32_t word = 0;
    for (uint32_t i = 0; i < num_weights; ++i) {
        word |= (weights[i] & 0xF) << (28 - 4 * (i % 8));
        if ((i % 8 == 7) || (i == num_weights - 1)) {
            uint32_t addr = WEIGHT_ADDR_OFFSET + 4 * (i / 8);
            CONV_WORD(addr) = word;
            xil_printf("Setting weight at addr 0x%08X: 0x%08X\n", CONV_BASE_ADDR + addr, word);
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
            xil_printf("Setting input at addr 0x%08X: 0x%08X\n", CONV_BASE_ADDR + addr, word);
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

uint32_t conv_get_result() {
    return CONV_WORD(RESULT_ADDR_OFFSET);
}

void conv_test() {
    uint8_t weights[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint8_t inputs[9]  = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    xil_printf("Setting convolution weights and inputs...\n");
    conv_set_weights(weights, 9);
    conv_set_inputs(inputs, 9);

    xil_printf("Starting convolution...\n");
    conv_start();

    conv_wait_done();

    uint32_t result = conv_get_result();
    xil_printf("Convolution result = 0x%d\n", result);
}
