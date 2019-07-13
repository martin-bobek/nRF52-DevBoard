#include <nrf.h>
#include "init.h"

extern "C" void RTC0_IRQHandler() __attribute((interrupt));

static inline void Sleep() __attribute((always_inline));

static volatile uint8_t LedsToggle = 0;

int main() {
    Init();
    __enable_irq();

    NRF_P0->OUTCLR = LED_ALL;

    for (uint8_t i = 0; i < 8; i++, LedsToggle = 0)
        while (!LedsToggle);

    NRF_P0->OUTSET = LED_ALL;

    for (uint8_t i = 0; i < 8; i++, LedsToggle = 0)
        while (!LedsToggle);

    // RE-ENABLE LEDS TO INDICATE TEST OUTCOME
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
    LedsToggle = 1;
}
