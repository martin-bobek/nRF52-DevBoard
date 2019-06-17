#include <nrf.h>
#include "init.h"

static inline void Sleep(void) __attribute((always_inline));

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
    static uint32_t expectedTick = 0;

#ifdef DEBUG
    if (NRF_RTC0->COUNTER != expectedTick) {
        __disable_irq();
        while (1);
    }
#endif

    expectedTick = (expectedTick + 1) & RTC_COUNTER_COUNTER_Msk;
    __disable_irq();
    while (NRF_RTC0->COUNTER != expectedTick) {
        __WFI();
        __enable_irq();
        __disable_irq();
    }

    __enable_irq();
}

void __attribute((interrupt)) RTC0_IRQHandler(void) {
    NRF_RTC0->EVENTS_TICK = 0;
}
