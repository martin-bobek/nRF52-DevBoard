#include <nrf.h>
#include "init.h"
#include "gpio_init.h"

#define REG(ADDR)       (*(volatile uint32_t *)ADDR)

#define ERRATA108_DST   REG(0x40000EE4)
#define ERRATA108_SRC   REG(0x10000258)
#define ERRATA108_MSK   0x0000004F

#define DCDC_ENABLED     MSK( POWER_DCDCEN_DCDCEN_Enabled,   POWER_DCDCEN_DCDCEN_Pos )
#define CLKSRC_XTAL      MSK( CLOCK_LFCLKSRC_SRC_Xtal,       CLOCK_LFCLKSRC_SRC_Pos  )

static constexpr uint32_t UARTE_PSEL_RTS =
        RTS_PIN                             << UARTE_PSEL_RTS_PIN_Pos     |
        UARTE_PSEL_RTS_CONNECT_Disconnected << UARTE_PSEL_RTS_CONNECT_Pos;
static constexpr uint32_t UARTE_PSEL_TXD =
        TXD_PIN                          << UARTE_PSEL_TXD_PIN_Pos     |
        UARTE_PSEL_TXD_CONNECT_Connected << UARTE_PSEL_TXD_CONNECT_Pos;
static constexpr uint32_t UARTE_PSEL_CTS =
        CTS_PIN                             << UARTE_PSEL_CTS_PIN_Pos     |
        UARTE_PSEL_CTS_CONNECT_Disconnected << UARTE_PSEL_CTS_CONNECT_Pos;
static constexpr uint32_t UARTE_PSEL_RXD =
        RXD_PIN                             << UARTE_PSEL_RXD_PIN_Pos     |
        UARTE_PSEL_RXD_CONNECT_Disconnected << UARTE_PSEL_RXD_CONNECT_Pos;
static constexpr uint32_t UARTE_BAUDRATE =
        UARTE_BAUDRATE_BAUDRATE_Baud9600 << UARTE_BAUDRATE_BAUDRATE_Pos;
static constexpr uint32_t UARTE_CONFIG =
        UARTE_CONFIG_HWFC_Disabled   << UARTE_CONFIG_HWFC_Pos   |
        UARTE_CONFIG_PARITY_Excluded << UARTE_CONFIG_PARITY_Pos;
static constexpr uint32_t UARTE_ENABLE =
        UARTE_ENABLE_ENABLE_Enabled << UARTE_ENABLE_ENABLE_Pos;

static constexpr uint32_t RTC_PRESCALER = 4095u;

static inline void PowerInit() __attribute((always_inline));
static inline void ErrataInit() __attribute((always_inline));
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
