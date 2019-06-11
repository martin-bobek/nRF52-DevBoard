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

    // RE-ENABLE LEDS TO INDICATE TEST OUTCOME
}

void __attribute((interrupt)) SysTick_Handler(void) {
    LedsToggle = 1;
}
