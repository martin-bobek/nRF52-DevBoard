#ifndef PORTMAP__
#define PORTMAP__

constexpr uint32_t BIT(uint8_t n) { return 1u << n; }

enum pin_t : uint8_t {
    RTS_PIN = 5,
    TXD_PIN,
    CTS_PIN,
    RXD_PIN,
    LED1_PIN = 17,
    LED2_PIN,
    LED3_PIN,
    LED4_PIN
};

constexpr uint32_t UART_RTS = BIT(RTS_PIN);
constexpr uint32_t UART_TXD = BIT(TXD_PIN);
constexpr uint32_t UART_CTS = BIT(CTS_PIN);
constexpr uint32_t UART_RXD = BIT(RXD_PIN);
constexpr uint32_t LED1     = BIT(LED1_PIN);
constexpr uint32_t LED2     = BIT(LED2_PIN);
constexpr uint32_t LED3     = BIT(LED3_PIN);
constexpr uint32_t LED4     = BIT(LED4_PIN);

#endif
