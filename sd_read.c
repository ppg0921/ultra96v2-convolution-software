#include "ff.h"
#include "xil_printf.h"
#include <string.h>
#include "sd_read.h"

#define MAX_LINE_LEN 128  // Each line of 8 weights = 8 chars + '\n'

UINT br;
FATFS fatfs;
FIL file;
char line[MAX_LINE_LEN];

int hexchar_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;  // invalid character
}

int sd_read_test() {
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
            int weight = hexchar_to_int(line[i]);
            if (weight < 0) {
                xil_printf("Invalid hex char: %c\n", line[i]);
                continue;
            }

            xil_printf("Weight[%d] = %d\n", i, weight);
            // You can now store/send weight as 4-bit value
        }
    }

    f_close(&file);
    return 0;
}
