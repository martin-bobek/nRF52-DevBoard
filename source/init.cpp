#include <nrf.h>
#include "init.h"
#include "gpio_init.h"
#include "uarte.h"

#define REG(ADDR)       (*(volatile uint32_t *)ADDR)

#define ERRATA108_DST   REG(0x40000EE4)
#define ERRATA108_SRC   REG(0x10000258)
#define ERRATA108_MSK   0x0000004F

#define DCDC_ENABLED     MSK( POWER_DCDCEN_DCDCEN_Enabled,   POWER_DCDCEN_DCDCEN_Pos )
#define CLKSRC_XTAL      MSK( CLOCK_LFCLKSRC_SRC_Xtal,       CLOCK_LFCLKSRC_SRC_Pos  )

static constexpr uint32_t RTC_PRESCALER = 4095u;

static inline void PowerInit() __attribute((always_inline));
static inline void ErrataInit() __attribute((always_inline));
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
    NVIC->ISER[0] = BIT(UARTE0_UART0_IRQn) | BIT(RTC0_IRQn);
}
