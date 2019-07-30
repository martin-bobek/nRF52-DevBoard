#include <nrf.h>
#include <cstddef>
#include "mutex.h"
#include "portmap.h"
#include "uarte.h"

extern "C" void UARTE0_UART0_IRQHandler() __attribute((interrupt));
static void StartTransfer();

struct Message {
    constexpr Message() : str(nullptr), length(0) {}
    constexpr Message(const char *_str, size_t _length) : str(_str), length(_length) {}
    const char *str;
    size_t length;
};

static constexpr uint32_t UARTE_INTEN =
        UARTE_INTEN_TXSTOPPED_Disabled << UARTE_INTEN_TXSTOPPED_Pos |
        UARTE_INTEN_TXSTARTED_Disabled << UARTE_INTEN_TXSTARTED_Pos |
        UARTE_INTEN_RXSTARTED_Disabled << UARTE_INTEN_RXSTARTED_Pos |
        UARTE_INTEN_RXTO_Disabled      << UARTE_INTEN_RXTO_Pos      |
        UARTE_INTEN_ERROR_Disabled     << UARTE_INTEN_ERROR_Pos     |
        UARTE_INTEN_ENDTX_Enabled      << UARTE_INTEN_ENDTX_Pos     |
        UARTE_INTEN_TXDRDY_Disabled    << UARTE_INTEN_TXDRDY_Pos    |
        UARTE_INTEN_ENDRX_Disabled     << UARTE_INTEN_ENDRX_Pos     |
        UARTE_INTEN_RXDRDY_Disabled    << UARTE_INTEN_RXDRDY_Pos    |
        UARTE_INTEN_NCTS_Disabled      << UARTE_INTEN_NCTS_Pos      |
        UARTE_INTEN_CTS_Disabled       << UARTE_INTEN_CTS_Pos;
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
static constexpr uint32_t BUFFER_FULL 	= BUFFER_LENGTH + 1;

static Message ptrBuffer[BUFFER_LENGTH];
static uint8_t head = 0;
static uint8_t tail = 0;
static Mutex txActive;


void UartInit() {
    NRF_UARTE0->INTEN = UARTE_INTEN;
    NRF_UARTE0->PSEL.RTS = UARTE_PSEL_RTS;
    NRF_UARTE0->PSEL.TXD = UARTE_PSEL_TXD;
    NRF_UARTE0->PSEL.CTS = UARTE_PSEL_CTS;
    NRF_UARTE0->PSEL.RXD = UARTE_PSEL_RXD;
    NRF_UARTE0->BAUDRATE = UARTE_BAUDRATE;
    NRF_UARTE0->CONFIG = UARTE_CONFIG;
    NRF_UARTE0->ENABLE = UARTE_ENABLE;
}

int SerialWrite(const char *str, size_t length) {
    if (head == BUFFER_FULL)
        return SERIAL_BUFFER_FULL;

    ptrBuffer[head] = { str, length };

    head++;
    if (head == BUFFER_LENGTH)
        head = 0;
    if (head == tail)
        head = BUFFER_FULL;

    if (txActive.Lock())
        StartTransfer();

    return SERIAL_SUCCESS;
}

void UARTE0_UART0_IRQHandler() {
    NRF_UARTE0->EVENTS_ENDTX = 0;

    if (head == BUFFER_FULL)
        head = tail;

    tail++;
    if (tail == BUFFER_LENGTH)
        tail = 0;

    if (tail == head)
        txActive.Unlock();
    else
        StartTransfer();
}

void StartTransfer() {
    NRF_UARTE0->TXD.PTR = (uint32_t)ptrBuffer[tail].str;
    NRF_UARTE0->TXD.MAXCNT = ptrBuffer[tail].length;
    NRF_UARTE0->TASKS_STARTTX = 1;
}
