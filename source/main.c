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

int main(void) {
    NRF_P0->OUT = 0;
    NRF_P0->PIN_CNF[LED1_PIN] = LED_PIN_CNF;
    NRF_P0->PIN_CNF[LED2_PIN] = LED_PIN_CNF;
    NRF_P0->PIN_CNF[LED3_PIN] = LED_PIN_CNF;
    NRF_P0->PIN_CNF[LED4_PIN] = LED_PIN_CNF;
    NRF_P0->DIRSET = LED_ALL;

    while (1) {
        for (volatile uint32_t i = 0; i < 3000000; i++);

        if (NRF_P0->OUT & LED_ALL)
            NRF_P0->OUTCLR = LED_ALL;
        else
            NRF_P0->OUTSET = LED_ALL;
    }
}
