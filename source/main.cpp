#include <nrf.h>
#include "init.h"
#include "uarte.h"

extern "C" void RTC0_IRQHandler() __attribute((interrupt));

static inline void LedThread() __attribute((always_inline));
static inline void UartThread() __attribute((always_inline));
static inline void Sleep() __attribute((always_inline));

int main() {
    Init();

    __enable_irq();
    while (true) {
        LedThread();
        UartThread();

        Sleep();
    }
}

void LedThread() {
    if (NRF_P0->OUT & LED_ALL)
        NRF_P0->OUTCLR = LED_ALL;
    else
        NRF_P0->OUTSET = LED_ALL;
}
void UartThread() {
    constexpr char str[] = "Hello World!\n\r";
    static uint8_t timer = 0;

    timer++;
    if (timer == 8) {
        timer = 0;

        SerialWrite(str, StringLen(str));
    }
}
void Sleep() {
    static uint32_t expectedTick = 0;

#ifndef NDEBUG
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
