/* Header guards that make sure the compiler doesnt read the file more than once and skips. */

#ifndef GPIO_H
#define GPIO_H

/* Gives explicit integer types like uint32_t (exact 32-bit unsigned int) */

#include <stdint.h>

/* Raspberry Pi 5 RP1 I/O Controller's base address designation */

#define RP1_BASE    0x1F00000000ULL /* ULL is the Unsigned Long Long and tells C the hex value needs 64 bits. */

#define GPIO_BASE   (RP1_BASE + 0x000D0000ULL) /* The GPIO controller that offsets inside the RP1 Chip of the Pi. */

/* Convert these raw hexidecimal numbers into hardware language. */

#define REG32(addr) (*(volatile uint32_t *)(uintptr_t)(addr))

/* Declaring the Function Prototypes - At the high level, we are telling a pin to turn on (High voltage = 1), or be off (Low voltage = 0) */

/* Each function takes a pin argument, no matter what number we pass using the uint32_t as an integer. */
void gpio_set_output(uint32_t pin);
void gpio_set_high(uint32_t pin);
void gpio_set_low(uint32_t pin);

/* Use this whever a conditional is used - here we used #ifndef - this ends a conditional compilation block. */
#endif