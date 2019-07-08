#include <cstddef>
#include <initializer_list>
#include <nrf.h>
#include "gpio_init.h"
#include "init.h"

static constexpr uint32_t HIDRV_BOTH =
        GPIO_PIN_CNF_DIR_Output     << GPIO_PIN_CNF_DIR_Pos   |
        GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos |
        GPIO_PIN_CNF_PULL_Disabled  << GPIO_PIN_CNF_PULL_Pos  |
        GPIO_PIN_CNF_DRIVE_H0H1     << GPIO_PIN_CNF_DRIVE_Pos |
        GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos;
static constexpr uint32_t INDSC_NOPL =
        GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos   |
        GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos |
        GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos  |
        GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos |
        GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos;
static constexpr uint32_t INDSC_PLUP =
        GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos   |
        GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos |
        GPIO_PIN_CNF_PULL_Pullup      << GPIO_PIN_CNF_PULL_Pos  |
        GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos |
        GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos;
static constexpr uint32_t INDSC_PLDWN =
        GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos   |
        GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos |
        GPIO_PIN_CNF_PULL_Pulldown    << GPIO_PIN_CNF_PULL_Pos  |
        GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos |
        GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos;
static constexpr uint32_t LED_PIN_CNF =
        GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos   |
        GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos |
        GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos  |
        GPIO_PIN_CNF_DRIVE_H0S1       << GPIO_PIN_CNF_DRIVE_Pos |
        GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos;

struct Element {
public:
    constexpr Element(size_t _pos, uint32_t _val) : pos(_pos), val(_val) {}
    size_t pos;
    uint32_t val;
};
template <size_t N, uint32_t Default>
class ArrayInit {
public:
    constexpr ArrayInit(const std::initializer_list<Element> list) : arr() {
        for (size_t i = 0; i < N; i++)
            arr[i] = Default;

        for (auto it = list.begin(); it < list.end(); it++)
            arr[it->pos] = it->val;
    }

    uint32_t operator[](size_t index) const { return arr[index]; }
private:
    uint32_t arr[N];
};

static constexpr ArrayInit<32, INDSC_PLDWN> GPIO_CNF = {
        { XL1_PIN,  INDSC_NOPL  },
        { XL2_PIN,  INDSC_NOPL  },
        { RTS_PIN,  INDSC_PLDWN },
        { TXD_PIN,  HIDRV_BOTH  },
        { CTS_PIN,  INDSC_NOPL  },
        { RXD_PIN,  INDSC_NOPL  },
        { LED1_PIN, LED_PIN_CNF },
        { LED2_PIN, LED_PIN_CNF },
        { LED3_PIN, LED_PIN_CNF },
        { LED4_PIN, LED_PIN_CNF },
        { RST_PIN,  INDSC_PLUP  },
};

void GpioInit() {
    NRF_P0->OUT = LED_ALL | UART_TXD;

    for (uint8_t pin = 0; pin < 32; pin++)
        NRF_P0->PIN_CNF[pin] = GPIO_CNF[pin];
}
