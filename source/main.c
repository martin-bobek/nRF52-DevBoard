#include <nrf.h>

#define MSK(value, shift)   (uint32_t)((value) << (shift))
#define BIT(n)              MSK(1, n)

#define LED1_PIN		17
#define LED2_PIN		18
#define LED3_PIN		19
#define LED4_PIN		20

#define LED1			BIT(LED1_PIN)
#define LED2			BIT(LED2_PIN)
#define LED3			BIT(LED3_PIN)
#define LED4			BIT(LED4_PIN)

#define LED_ALL         (LED1 | LED2 | LED3 | LED4)

#define LED_PIN_CNF     (MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos ) | \
                         MSK( GPIO_PIN_CNF_DRIVE_H0S1,       GPIO_PIN_CNF_DRIVE_Pos ))

#define SYSTICK_250MS   (16000000u - 1u)

static volatile uint8_t LedsToggle = 0;

int main(void) {
    NRF_P0->OUT = 0;
    NRF_P0->PIN_CNF[LED1_PIN] = LED_PIN_CNF;
    NRF_P0->PIN_CNF[LED2_PIN] = LED_PIN_CNF;
    NRF_P0->PIN_CNF[LED3_PIN] = LED_PIN_CNF;
    NRF_P0->PIN_CNF[LED4_PIN] = LED_PIN_CNF;
    NRF_P0->DIRSET = LED_ALL;

    SysTick->LOAD = SYSTICK_250MS;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    __enable_irq();
    while (1) {
        if (NRF_P0->OUT & LED_ALL)
            NRF_P0->OUTCLR = LED_ALL;
        else
            NRF_P0->OUTSET = LED_ALL;

        while (!LedsToggle);
        LedsToggle = 0;
    }
}

void __attribute((interrupt)) SysTick_Handler(void) {
    LedsToggle = 1;
}
