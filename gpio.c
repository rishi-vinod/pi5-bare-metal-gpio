#include "gpio.h"

#define RIO_BASE   (RP1_BASE + 0x000E0000ULL)
#define RIO_OUT_SET    (RIO_BASE + 0x24)
/* Atomic set register */
#define RIO_OUT_CLR   (RIO_BASE + 0x28)
/* Atomic clear register */
void gpio_set_output(uint32_t pin) {
    uintptr_t ctrl_reg = GPIO_BASE + (pin*8)+4; /* Pin times 8 because the structure of the Pi's chip has 8 bytes for one register, 4 is in the middle of this called the control register. */
    REG32(ctrl_reg) = 5; 
}

void gpio_set_high(uint32_t pin) {
    REG32(RIO_OUT_SET) = (1U << pin); /* 1U is the number 1 as an unsigned binary integer and << slides the 1 to left "pin" times. */
}

void gpio_set_low(uint32_t pin) {
    REG32(RIO_OUT_CLR) = (1U << pin); /* Pulls down voltage to 0V */
}