#include <nrf.h>
#include "portmap.h"
#include "uarte.h"

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

static constexpr uint32_t BUFFER_LENGTH = 32;
static constexpr uint32_t BUFFER_FULL 	= BUFFER_LENGTH;

static const char *ptrBuffer[BUFFER_LENGTH];
static uint8_t head = 0;
static uint8_t tail = 0;


void UartInit() {
    NRF_UARTE0->PSEL.RTS = UARTE_PSEL_RTS;
    NRF_UARTE0->PSEL.TXD = UARTE_PSEL_TXD;
    NRF_UARTE0->PSEL.CTS = UARTE_PSEL_CTS;
    NRF_UARTE0->PSEL.RXD = UARTE_PSEL_RXD;
    NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE;
    NRF_UARTE0->CONFIG = UARTE_CONFIG;
    NRF_UARTE0->ENABLE = UARTE_ENABLE;
}

int SerialWrite(const char *str) {
    if (head == BUFFER_FULL)
        return SERIAL_BUFFER_FULL;

    ptrBuffer[head] = str;

    head++;
    if (head == BUFFER_LENGTH)
        head = 0;
    if (head == tail)
        head = BUFFER_FULL;

    return SERIAL_SUCCESS;
}
