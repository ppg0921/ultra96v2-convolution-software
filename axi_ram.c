#include "axi_ram.h"
#include "xil_printf.h"

void ram_write(uint32_t addr, uint32_t data) {
    RAM_WORD(addr) = data;
}

uint32_t ram_read(uint32_t addr) {
    return RAM_WORD(addr);
}

void ram_test() {
    // print("Starting AXI RAM test...\n");

    uint32_t test_addr[] = {0xFFF0, 0xFFF4, 0xFFF8, 0xFFFC, 0xFF10};
    uint32_t test_data[] = {0xDEADBEEF, 0x12345678, 0xCAFEBABE, 0x0BADBEEF, 0xFEEDC0DE};
    uint32_t read_back;

    print("test good read\n");
    xil_printf("Test: reading from SLCR at 0xFF180000: %08X\n", *((volatile uint32_t *)0xFF180000));
    print("after test good read\n");

    for (int i = 0; i < 5; ++i) {
//    	xil_printf("i = %d\n", i);
        ram_write(test_addr[i], test_data[i]);
        xil_printf("after writing\n");
        read_back = ram_read(test_addr[i]);
        xil_printf("Write 0x%08X to 0x%08X, Read 0x%08X\n", test_data[i], RAM_BASE_ADDR + test_addr[i], read_back);
        if (read_back != test_data[i]) {
            xil_printf("RAM test failed at address 0x%08X\n", RAM_BASE_ADDR + test_addr[i]);
            return;
        }
    }

    print("AXI RAM test passed!\n");
}
