#include <nrf.h>
#include "init.h"

#define INDSC_PLUP      (MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos ) | \
                         MSK( GPIO_PIN_CNF_PULL_Pullup,      GPIO_PIN_CNF_PULL_Pos  ))
#define INDSC_PLDWN     (MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos ) | \
                         MSK( GPIO_PIN_CNF_PULL_Pulldown,    GPIO_PIN_CNF_PULL_Pos  ))
#define LED_PIN_CNF     (MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos ) | \
                         MSK( GPIO_PIN_CNF_DRIVE_H0S1,       GPIO_PIN_CNF_DRIVE_Pos ))

#define SYSTICK_250MS   (16000000u - 1u)

const uint32_t GPIO_CNF[32] = {
//      P0.00        P0.01        P0.02        P0.03        P0.04        P0.05 CTS    P0.06 RxD    P0.07 RTS
        INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLUP,  INDSC_PLUP,  INDSC_PLUP,
//      P0.08 TxD    P0.09 NFC1   P0.10 NFC2   P0.11        P0.12        P0.13 BTN1   P0.14 BTN2   P0.15 BTN3
        INDSC_PLUP,  INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN,
//      P0.16 BTN4   P0.17 LED1   P0.18 LED2   P0.19 LED3   P0.20 LED4   P0.21 RESET  P0.22        P0.23
        INDSC_PLDWN, LED_PIN_CNF, LED_PIN_CNF, LED_PIN_CNF, LED_PIN_CNF, INDSC_PLUP,  INDSC_PLDWN, INDSC_PLDWN,
//      P0.24        P0.25        P0.26        P0.27        P0.28        P0.29        P0.30        P0.31
        INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN };

void Init(void) {
    for (uint8_t pin = 0; pin < 32; pin++)
        NRF_P0->PIN_CNF[pin] = GPIO_CNF[pin];

    NRF_P0->OUT = 0;
    NRF_P0->DIRSET = LED_ALL;

    SysTick->LOAD = SYSTICK_250MS;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}
