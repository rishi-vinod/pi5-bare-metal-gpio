#include "gpio.h"

void delay(volatile uint32_t count) {
    while(count--) {
        asm volatile("nop");
    }
}

void kernel_main(void) {
    gpio_set_output(9); /* 9 is the onboard LED on the Pi */

    while(1) { /* Since there is no operating system - we need a way for the command following this to continue without the CPU crashing. */
        gpio_set_high(9);
        delay(300000000);
        gpio_set_low(9);
        delay(300000000);
    }
}