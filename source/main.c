#include <nrf.h>
#include "init.h"

static inline void Sleep(void) __attribute((always_inline));

static volatile uint8_t LedsToggle = 0;

int main(void) {
    Init();

    __enable_irq();
    while (1) {
        if (NRF_P0->OUT & LED_ALL)
            NRF_P0->OUTCLR = LED_ALL;
        else
            NRF_P0->OUTSET = LED_ALL;

        Sleep();
    }
}

void Sleep(void) {
    while (!LedsToggle);

    LedsToggle = 0;
}

void __attribute((interrupt)) SysTick_Handler(void) {
    LedsToggle = 1;
}
