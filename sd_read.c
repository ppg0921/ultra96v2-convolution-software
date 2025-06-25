#include "ff.h"
#include "xil_printf.h"
#include <string.h>
#include "sd_read.h"

#define MAX_LINE_LEN 831744  // Each line of 8 weights = 8 chars + '\n'

UINT br;
FATFS fatfs;
FIL file;
char line[MAX_LINE_LEN];

uint8_t hexchar_to_int(char c) {
    uint8_t v;
    if (c >= '0' && c <= '9') v = c - '0';
    if (c >= 'A' && c <= 'F') v = c - 'A' + 0x0A;
    if (c >= 'a' && c <= 'f') v = c - 'a' + 0x0A;
    return v;  // invalid character
}

int sd_read_inputs(uint8_t *input_buffer, uint8_t *weight_buffer) {
    FRESULT fr;
    fr = f_mount(&fatfs, "0:/", 1);
    if (fr != FR_OK) {
        xil_printf("Failed to mount SD (%d)\n", fr);
        return -1;
    }

    fr = f_open(&file, "weights.txt", FA_READ);
    if (fr != FR_OK) {
        xil_printf("Failed to open file (%d)\n", fr);
        return -2;
    }

    xil_printf("Reading weights:\n");

    while (f_gets(line, sizeof(line), &file)) {
        int len = strlen(line);

        // Strip newline characters
        if (line[len - 1] == '\n' || line[len - 1] == '\r') line[--len] = 0;
        if (line[len - 1] == '\r') line[--len] = 0;

        for (int i = 0; i < len; i++) {
            uint8_t weight = hexchar_to_int(line[i]);
            if (weight > 0x0F) {
                xil_printf("Invalid hex char: %c\n", line[i]);
                continue;
            }

            xil_printf("Weight[%d] = %d\n", i, weight);
            // You can now store/send weight as 4-bit value
            
            weight_buffer[i] = weight;  // Store in weight buffer
            
        }
    }

    f_close(&file);

    fr = f_open(&file, "x.txt", FA_READ);
    if (fr != FR_OK) {
        xil_printf("Failed to open file (%d)\n", fr);
        return -2;
    }

    xil_printf("Reading x:\n");

    while (f_gets(line, sizeof(line), &file)) {
        int len = strlen(line);

        // Strip newline characters
        if (line[len - 1] == '\n' || line[len - 1] == '\r') line[--len] = 0;
        if (line[len - 1] == '\r') line[--len] = 0;

        for (int i = 0; i < len; i++) {
            uint8_t x = hexchar_to_int(line[i]);
            if (x > 0x0F) {
                xil_printf("Invalid hex char: %c\n", line[i]);
                continue;
            }

            xil_printf("X[%d] = %d\n", i, x);
            // You can now store/send x as 4-bit value

            input_buffer[i] = x;  // Store in input buffer
        }
    }

    return 0;
}