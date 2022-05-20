#include <stdio.h>
#include <unistd.h>
#include "mcp23s17.h"


int main(void)
{
    const int bus = 0;
    const int chip_select = 0;
    const int hw_addr = 0;

    int mcp23s17_fd = mcp23s17_open(bus, chip_select);

    // config register
    const uint8_t ioconfig = BANK_OFF | \
                             INT_MIRROR_OFF | \
                             SEQOP_OFF | \
                             DISSLW_OFF | \
                             HAEN_ON | \
                             ODR_OFF | \
                             INTPOL_LOW;
    mcp23s17_write_reg(ioconfig, IOCON, hw_addr, mcp23s17_fd);

    // I/O direction
    mcp23s17_write_reg(0xFF, IODIRA, hw_addr, mcp23s17_fd);
    mcp23s17_write_reg(0x00, IODIRB, hw_addr, mcp23s17_fd);

    // GPIOA pull ups
    mcp23s17_write_reg(0xff, GPPUA, hw_addr, mcp23s17_fd);

    // Write 0xaa to GPIO Port B
    mcp23s17_write_reg(0x00, GPIOB, hw_addr, mcp23s17_fd);

    struct mcp23s17_interrupt gpio_intr_desc;
    struct epoll_event event;
    gpio_intr_desc.gpio_pin_no = 17;
    gpio_intr_desc.edge = rising_edge;


    if( mcp23s17_enable_interrupts(&gpio_intr_desc) == 0 )
    {
    	printf("Waiting for interrupt...\n");
        if (mcp23s17_wait_for_interrupt(&gpio_intr_desc, &event, 100000))
        {
        	// print the input state
        	uint8_t input = mcp23s17_read_reg(GPIOA, hw_addr, mcp23s17_fd);
        	printf("Inputs: 0x%x\n", input);
        }
		else
		{
			printf("Too long waiting for inputs!\n");
		}
    }

    mcp23s17_disable_interrupts(&gpio_intr_desc);

    close(mcp23s17_fd);
}
