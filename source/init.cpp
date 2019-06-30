#include <nrf.h>
#include "init.h"

#define REG(ADDR)       (*(volatile uint32_t *)ADDR)

#define ERRATA108_DST   REG(0x40000EE4)
#define ERRATA108_SRC   REG(0x10000258)
#define ERRATA108_MSK   0x0000004F

#define HIDRV_BOTH       MSK( GPIO_PIN_CNF_DRIVE_H0H1,       GPIO_PIN_CNF_DRIVE_Pos  )
#define INDSC_NOPL       MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos  )
#define INDSC_PLUP      (MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos  ) | \
                         MSK( GPIO_PIN_CNF_PULL_Pullup,      GPIO_PIN_CNF_PULL_Pos   ))
#define INDSC_PLDWN     (MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos  ) | \
                         MSK( GPIO_PIN_CNF_PULL_Pulldown,    GPIO_PIN_CNF_PULL_Pos   ))
#define LED_PIN_CNF     (MSK( GPIO_PIN_CNF_INPUT_Disconnect, GPIO_PIN_CNF_INPUT_Pos  ) | \
                         MSK( GPIO_PIN_CNF_DRIVE_H0S1,       GPIO_PIN_CNF_DRIVE_Pos  ))
#define DCDC_ENABLED     MSK( POWER_DCDCEN_DCDCEN_Enabled,   POWER_DCDCEN_DCDCEN_Pos )
#define CLKSRC_XTAL      MSK( CLOCK_LFCLKSRC_SRC_Xtal,       CLOCK_LFCLKSRC_SRC_Pos  )

static constexpr uint32_t UARTE_PSEL_RTS =
        MSK( RTS_PIN,                             UARTE_PSEL_RTS_PIN_Pos     ) |
        MSK( UARTE_PSEL_RTS_CONNECT_Disconnected, UARTE_PSEL_RTS_CONNECT_Pos );
static constexpr uint32_t UARTE_PSEL_TXD =
        MSK( TXD_PIN,                          UARTE_PSEL_TXD_PIN_Pos     ) |
        MSK( UARTE_PSEL_TXD_CONNECT_Connected, UARTE_PSEL_TXD_CONNECT_Pos );
static constexpr uint32_t UARTE_PSEL_CTS =
        MSK( CTS_PIN,                             UARTE_PSEL_CTS_PIN_Pos     ) |
        MSK( UARTE_PSEL_CTS_CONNECT_Disconnected, UARTE_PSEL_CTS_CONNECT_Pos );
static constexpr uint32_t UARTE_PSEL_RXD =
        MSK( RXD_PIN,                             UARTE_PSEL_RXD_PIN_Pos     ) |
        MSK( UARTE_PSEL_RXD_CONNECT_Disconnected, UARTE_PSEL_RXD_CONNECT_Pos );
static constexpr uint32_t UARTE_BAUDRATE =
        MSK( UARTE_BAUDRATE_BAUDRATE_Baud9600, UARTE_BAUDRATE_BAUDRATE_Pos );
static constexpr uint32_t UARTE_CONFIG =
        MSK( UARTE_CONFIG_HWFC_Disabled,   UARTE_CONFIG_HWFC_Pos   ) |
        MSK( UARTE_CONFIG_PARITY_Excluded, UARTE_CONFIG_PARITY_Pos );
static constexpr uint32_t UARTE_ENABLE =
        MSK( UARTE_ENABLE_ENABLE_Enabled, UARTE_ENABLE_ENABLE_Pos );

static constexpr uint32_t RTC_PRESCALER = 4095u;

static const uint32_t GPIO_CNF[32] = {
//      P0.00 XL1    P0.01 XL2    P0.02        P0.03        P0.04        P0.05 RTS    P0.06 TxD    P0.07 CTS
        INDSC_NOPL,  INDSC_NOPL,  INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, HIDRV_BOTH,  INDSC_NOPL,
//      P0.08 RxD    P0.09 NFC1   P0.10 NFC2   P0.11        P0.12        P0.13 BTN1   P0.14 BTN2   P0.15 BTN3
        INDSC_NOPL,  INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN,
//      P0.16 BTN4   P0.17 LED1   P0.18 LED2   P0.19 LED3   P0.20 LED4   P0.21 RESET  P0.22        P0.23
        INDSC_PLDWN, LED_PIN_CNF, LED_PIN_CNF, LED_PIN_CNF, LED_PIN_CNF, INDSC_PLUP,  INDSC_PLDWN, INDSC_PLDWN,
//      P0.24        P0.25        P0.26        P0.27        P0.28        P0.29        P0.30        P0.31
        INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN, INDSC_PLDWN };

static inline void PowerInit() __attribute((always_inline));
static inline void ErrataInit() __attribute((always_inline));
static inline void GpioInit() __attribute((always_inline));
static inline void UartInit() __attribute((always_inline));
static inline void ClockInit() __attribute((always_inline));
static inline void RtcInit() __attribute((always_inline));
static inline void IsrInit() __attribute((always_inline));

void Init() {
    PowerInit();
    ErrataInit();
    GpioInit();
    ClockInit();
    UartInit();
    RtcInit();
    IsrInit();
}

void PowerInit() {
    NRF_POWER->DCDCEN = DCDC_ENABLED;
}
void ErrataInit() {
    ERRATA108_DST = ERRATA108_SRC & ERRATA108_MSK;
}
void GpioInit() {
    for (uint8_t pin = 0; pin < 32; pin++)
        NRF_P0->PIN_CNF[pin] = GPIO_CNF[pin];

    NRF_P0->OUT = LED_ALL | UART_TXD;
    NRF_P0->DIR = LED_ALL | UART_TXD;
}
void UartInit() {
    NRF_UARTE0->PSEL.RTS = UARTE_PSEL_RTS;
    NRF_UARTE0->PSEL.TXD = UARTE_PSEL_TXD;
    NRF_UARTE0->PSEL.CTS = UARTE_PSEL_CTS;
    NRF_UARTE0->PSEL.RXD = UARTE_PSEL_RXD;
    NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE;
    NRF_UARTE0->CONFIG = UARTE_CONFIG;
    NRF_UARTE0->ENABLE = UARTE_ENABLE;
}
void ClockInit() {
    NRF_CLOCK->LFCLKSRC = CLKSRC_XTAL;
    NRF_CLOCK->TASKS_LFCLKSTART = 1;
}
void RtcInit() {
    NRF_RTC0->PRESCALER = RTC_PRESCALER;
    NRF_RTC0->INTENSET = RTC_INTENSET_TICK_Msk;
    NRF_RTC0->TASKS_START = 1;
}
void IsrInit() {
    NVIC->ISER[0] = BIT(RTC0_IRQn);
}
