#include <nrf.h>
#include "init.h"

extern "C" void RTC0_IRQHandler() __attribute((interrupt));

static void Sleep();

int main() {
    Init();
    __enable_irq();

    NRF_P0->OUTCLR = LED_ALL;

    for (uint8_t i = 0; i < 8; i++)
        Sleep();

    NRF_P0->OUTSET = LED_ALL;

    for (uint8_t i = 0; i < 8; i++)
        Sleep();

    // RE-ENABLE LEDS TO INDICATE TEST OUTCOME

    while (true)
        NRF_POWER->SYSTEMOFF = 1;
}

void Sleep() {
    static uint32_t expectedTick = 0;

#ifdef DEBUG
    if (NRF_RTC0->COUNTER != expectedTick) {
        __disable_irq();
        while (true);
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

void RTC0_IRQHandler() {
    NRF_RTC0->EVENTS_TICK = 0;
}
