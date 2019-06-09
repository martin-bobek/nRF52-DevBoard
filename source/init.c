#include <nrf.h>
#include "init.h"

#define INPUT_DISC       MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos )
#define LED_PIN_CNF     (MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos ) | \
                         MSK( GPIO_PIN_CNF_DRIVE_H0S1,       GPIO_PIN_CNF_DRIVE_Pos ))

#define SYSTICK_250MS   (16000000u - 1u)

const uint32_t GPIO_CNF[32] = {
//      P0.00       P0.01        P0.02        P0.03        P0.04        P0.05       P0.06       P0.07
        INPUT_DISC, INPUT_DISC,  INPUT_DISC,  INPUT_DISC,  INPUT_DISC,  INPUT_DISC, INPUT_DISC, INPUT_DISC,
//      P0.08       P0.09        P0.10        P0.11        P0.12        P0.13       P0.14       P0.15
        INPUT_DISC, INPUT_DISC,  INPUT_DISC,  INPUT_DISC,  INPUT_DISC,  INPUT_DISC, INPUT_DISC, INPUT_DISC,
//      P0.16       P0.17        P0.18        P0.19        P0.20        P0.21       P0.22       P0.23
        INPUT_DISC, LED_PIN_CNF, LED_PIN_CNF, LED_PIN_CNF, LED_PIN_CNF, INPUT_DISC, INPUT_DISC, INPUT_DISC,
//      P0.24       P0.25        P0.26        P0.27        P0.28        P0.29       P0.30       P0.31
        INPUT_DISC, INPUT_DISC,  INPUT_DISC,  INPUT_DISC,  INPUT_DISC,  INPUT_DISC, INPUT_DISC, INPUT_DISC };

void Init(void) {
    for (uint8_t pin = 0; pin < 32; pin++)
        NRF_P0->PIN_CNF[pin] = GPIO_CNF[pin];

    NRF_P0->OUT = 0;
    NRF_P0->DIRSET = LED_ALL;

    SysTick->LOAD = SYSTICK_250MS;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}
