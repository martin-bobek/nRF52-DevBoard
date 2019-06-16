#include <nrf.h>
#include "init.h"

static volatile uint8_t LedsToggle = 0;

int main(void) {
    Init();
    __enable_irq();

    for (uint8_t i = 0; i < 4; i++, LedsToggle = 0)
        while (!LedsToggle);

    NRF_P0->OUTSET = LED_ALL;

    for (uint8_t i = 0; i < 4; i++, LedsToggle = 0)
        while (!LedsToggle);

    if (NVIC->IP[0] == 0)       // IPx is reset to 0.
        NRF_P0->OUTCLR = LED1;
    NVIC->IP[0] = 0xFF;
    if (NVIC->IP[0] & 0xE0)     // 8 priority levels are supported (3-bit).
        NRF_P0->OUTCLR = LED2;
}

void __attribute((interrupt)) SysTick_Handler(void) {
    LedsToggle = 1;
}
