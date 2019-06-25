#include <nrf.h>
#include "init.h"

#define REG(ADDR)       (*(volatile uint32_t *)ADDR)

#define ERRATA108_DST   REG(0x40000EE4)
#define ERRATA108_SRC   REG(0x10000258)
#define ERRATA108_MSK   0x0000004F

#define INDSK_NOPL       MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos )
#define INDSC_PLUP      (MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos ) | \
                         MSK( GPIO_PIN_CNF_PULL_Pullup,      GPIO_PIN_CNF_PULL_Pos  ))
#define INDSC_PLDWN     (MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos ) | \
                         MSK( GPIO_PIN_CNF_PULL_Pulldown,    GPIO_PIN_CNF_PULL_Pos  ))
#define LED_PIN_CNF     (MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos ) | \
                         MSK( GPIO_PIN_CNF_DRIVE_H0S1,       GPIO_PIN_CNF_DRIVE_Pos ))
#define DCDC_ENABLED     MSK( POWER_DCDCEN_DCDCEN_Enabled,   POWER_DCDCEN_DCDCEN_Pos )
#define CLKSRC_XTAL      MSK( CLOCK_LFCLKSRC_SRC_Xtal,   CLOCK_LFCLKSRC_SRC_Pos )

#define RTC_PRESCALER   4095u

static const uint32_t GPIO_CNF[32] = {
//      P0.00 XL1    P0.01 XL2    P0.02        P0.03        P0.04        P0.05 CTS    P0.06 RxD    P0.07 RTS
        INDSK_NOPL,  INDSK_NOPL,  INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLUP,  INDSC_PLUP,  INDSC_PLUP,
//      P0.08 TxD    P0.09 NFC1   P0.10 NFC2   P0.11        P0.12        P0.13 BTN1   P0.14 BTN2   P0.15 BTN3
        INDSC_PLUP,  INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN,
//      P0.16 BTN4   P0.17 LED1   P0.18 LED2   P0.19 LED3   P0.20 LED4   P0.21 RESET  P0.22        P0.23
        INDSC_PLDWN, LED_PIN_CNF, LED_PIN_CNF, LED_PIN_CNF, LED_PIN_CNF, INDSC_PLUP,  INDSC_PLDWN, INDSC_PLDWN,
//      P0.24        P0.25        P0.26        P0.27        P0.28        P0.29        P0.30        P0.31
        INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN };

static inline void PowerInit(void) __attribute((always_inline));
static inline void ErrataInit(void) __attribute((always_inline));
static inline void GpioInit(void) __attribute((always_inline));
static inline void ClockInit(void) __attribute((always_inline));
static inline void RtcInit(void) __attribute((always_inline));
static inline void IsrInit(void) __attribute((always_inline));

void Init(void) {
    PowerInit();
    ErrataInit();
    GpioInit();
    ClockInit();
    RtcInit();
    IsrInit();
}

void PowerInit(void) {
    NRF_POWER->DCDCEN = DCDC_ENABLED;
}
void ErrataInit(void) {
    ERRATA108_DST = ERRATA108_SRC & ERRATA108_MSK;
}
void GpioInit(void) {
    for (uint8_t pin = 0; pin < 32; pin++)
        NRF_P0->PIN_CNF[pin] = GPIO_CNF[pin];

    NRF_P0->OUT = LED_ALL;
    NRF_P0->DIRSET = LED_ALL;
}
void ClockInit(void) {
    NRF_CLOCK->LFCLKSRC = CLKSRC_XTAL;
    NRF_CLOCK->TASKS_LFCLKSTART = 1;
}
void RtcInit(void) {
    NRF_RTC0->PRESCALER = RTC_PRESCALER;
    NRF_RTC0->INTENSET = RTC_INTENSET_TICK_Msk;
    NRF_RTC0->TASKS_START = 1;
}
void IsrInit(void) {
    NVIC->ISER[0] = BIT(RTC0_IRQn);
}
