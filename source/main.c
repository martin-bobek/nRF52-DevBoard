#include <nrf.h>
#include "init.h"

static volatile uint8_t LedsToggle = 0;

int main(void) {
    Init();

    while (1)
        NRF_POWER->SYSTEMOFF = 1;
}

void __attribute((interrupt)) SysTick_Handler(void) {
    LedsToggle = 1;
}
