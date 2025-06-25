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

void conv_set_input1(const uint8_t *inputs, uint32_t num_inputs) {
    uint32_t word = 0;
    for (uint32_t i = 0; i < num_inputs; ++i) {
        word |= (inputs[i] & 0xF) << (28 - 4 * (i % 8));
        if ((i % 8 == 7) || (i == num_inputs - 1)) {
            uint32_t addr = INPUT12_ADDR_OFFSET + 4 * (i / 8);
            CONV_WORD(addr) = word;
            xil_printf("Setting input at addr 0x%08X: 0x%08X\n", addr, word);
            word = 0;
        }
    }
}

void conv_set_input2(const uint8_t *inputs, uint32_t num_inputs) {
    uint32_t word = 0;
    for (uint32_t i = 0; i < num_inputs; ++i) {
        word |= (inputs[i] & 0xF) << (28 - 4 * (i % 8));
        if ((i % 8 == 7) || (i == num_inputs - 1)) {
            uint32_t addr = INPUT2_ADDR_OFFSET + 4 * (i / 8);
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

void conv_clear() {
    CONV_WORD(CLEAR_ADDR_OFFSET) = 1;
}

uint32_t conv_get_result(uint32_t unit_index) {
    return CONV_WORD(RESULT_ADDR_OFFSET + 4 * unit_index);
}

void conv_test_multiple_units() {
    xil_printf("=== Multi-Unit Convolution Test ===\n");

    uint8_t weights[IN_CHANNELS * INPUT_SIZE_PER_UNIT];
    uint8_t input_1[IN_CHANNELS * INPUT_SIZE_PER_UNIT];
    uint8_t input_2[IN_CHANNELS * INPUT_SIZE_PER_UNIT];

    uint8_t total_weights[IN_CHANNELS * OUT_CHANNELS * INPUT_SIZE_PER_UNIT]; // 64 * 64 * 9
    // Weight for first output channel is from 0 to 64*9-1, 
    // second output channel from 64*9 to 2*(64*9)-1, etc.


    uint8_t total_inputs[IN_CHANNELS  * INPUT_SIZE];// (112 * 112)*64
    // first image from 0 to 112*112-1, 
    // second image from 112*112 to 2*(112*112)-1, etc.

    // Fill total_weights and total_inputs with some test data
    


    // calculate 1 - 64 output channels one at a time
    for (uint32_t channel = 0; channel < OUT_CHANNELS; ++channel){
        

        // Fill weights for the current output channel
        // Fill 64 x 3 x 3 weight matrix
        for (uint32_t i = 0; i < IN_CHANNELS; ++i) {
            for (uint32_t j = 0; j < INPUT_SIZE_PER_UNIT; ++j) {
                weights[i * INPUT_SIZE_PER_UNIT + j] = total_weights[channel * (IN_CHANNELS * INPUT_SIZE_PER_UNIT) + i * INPUT_SIZE_PER_UNIT + j];
            }
        }
        // for each channel, calculate the output sized 112x112, total 112*112 / 2 iterations
        for (uint32_t iterations = 0; iterations < OUTPUT_SIZE; iterations += 2) {

            // Fill 64 x 3 x 3 input matrix for the current iteration
            for (uint32_t i = 0; i < IN_CHANNELS; ++i){
                // first input should be 
                    // conv engine 1: 
                    //      [[0, 1, 2], [114, 115, 116], [228, 229, 230]] + (114 * 114 * 0) 
                    // conv engine 2:
                    //      [[0, 1, 2], [114, 115, 116], [224, 225, 226]] + (114 * 114 * 1) 
                    // conv engine 3:
                    //      [[0, 1, 2], [114, 115, 116], [224, 225, 226]] + (114 * 114 * 2) 
                    // conv engine 4:
                for (uint32_t j = 0; j < 3 ; ++j) {
                    for (uint32_t k = 0; k < 3 ; ++k) {
                        input_1[i * INPUT_SIZE_PER_UNIT + j * 3 + k] = total_inputs[i * INPUT_SIZE + j * 114 + k];
                        input_2[i * INPUT_SIZE_PER_UNIT + j * 3 + k] = total_inputs[i * INPUT_SIZE + j * 114 + k + 1];
                    }
                }
            }

            // set inputs set weights
            conv_set_weights(weights, IN_CHANNELS * INPUT_SIZE_PER_UNIT);
            conv_set_input1(input_1, IN_CHANNELS * INPUT_SIZE_PER_UNIT);
            conv_set_input2(input_2, IN_CHANNELS * INPUT_SIZE_PER_UNIT);

            xil_printf("Weights and Inputs set. Starting convolution...\n");
            conv_start();
            conv_wait_done();

            xil_printf("Convolution done. Reading results:\n");

            for (uint32_t i = 0; i < NUM_UNITS; ++i) {
                uint32_t result = conv_get_result(i);
                xil_printf("Result[%2d] = 0x%d\n", i, result);
            }
        }
        

    }
}
