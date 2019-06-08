#include <nrf.h>
#include "init.h"

#define LED_PIN_CNF     (MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos ) | \
                         MSK( GPIO_PIN_CNF_DRIVE_H0S1,       GPIO_PIN_CNF_DRIVE_Pos ))

#define SYSTICK_250MS   (16000000u - 1u)

void Init(void) {
    NRF_P0->OUT = 0;
    NRF_P0->PIN_CNF[LED1_PIN] = LED_PIN_CNF;
    NRF_P0->PIN_CNF[LED2_PIN] = LED_PIN_CNF;
    NRF_P0->PIN_CNF[LED3_PIN] = LED_PIN_CNF;
    NRF_P0->PIN_CNF[LED4_PIN] = LED_PIN_CNF;
    NRF_P0->DIRSET = LED_ALL;

    SysTick->LOAD = SYSTICK_250MS;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}
