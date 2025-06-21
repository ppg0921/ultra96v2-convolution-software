#include "axi_ddr.h"
#include "xil_printf.h"

void ddr_write(uint32_t addr, uint32_t data) {
    DDR_WORD(addr) = data;
}

uint32_t ddr_read(uint32_t addr) {
    return DDR_WORD(addr);
}

void ddr_test() {
    xil_printf("Starting DDR test...\n");

    uint32_t test_addr[] = {0x0, 0x4, 0x8, 0xC, 0x10};
    uint32_t test_data[] = {0xCAFEBABE, 0x12345678, 0xDEADBEEF, 0x0BADCAFE, 0xFEEDC0DE};
    uint32_t read_back;

    for (int i = 0; i < 5; ++i) {
        ddr_write(test_addr[i], test_data[i]);
        read_back = ddr_read(test_addr[i]);
        xil_printf("Write 0x%08X to DDR[0x%08X], Read = 0x%08X\n", test_data[i], DDR_BASE_ADDR + test_addr[i], read_back);

        if (read_back != test_data[i]) {
            xil_printf("DDR test failed at address 0x%08X\n", DDR_BASE_ADDR + test_addr[i]);
            return;
        }
    }

    xil_printf("DDR test passed!\n");
}