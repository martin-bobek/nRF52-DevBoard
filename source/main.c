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

    if (NRF_UICR->PSELRESET[0] == 21)
        NRF_P0->OUTCLR = LED1;
    if (NRF_UICR->PSELRESET[0] == 21)
        NRF_P0->OUTCLR = LED2;
    if (NRF_UICR->APPROTECT == ~0)
        NRF_P0->OUTCLR = LED3;
    if (NRF_UICR->NFCPINS == ~1)
        NRF_P0->OUTCLR = LED4;
}

void __attribute((interrupt)) SysTick_Handler(void) {
    LedsToggle = 1;
}
