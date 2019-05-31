#include <nrf.h>

#define BIT(n) (uint32_t)(1u << n)

#define LED1_PIN		17
#define LED2_PIN		18
#define LED3_PIN		19
#define LED4_PIN		20

#define LED1			BIT(LED1_PIN)
#define LED2			BIT(LED2_PIN)
#define LED3			BIT(LED3_PIN)
#define LED4			BIT(LED4_PIN)

int main(void) {
    NRF_P0->DIRSET = LED1 | LED2 | LED3 | LED4;
}
