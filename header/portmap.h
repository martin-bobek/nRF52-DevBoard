#ifndef PORTMAP__
#define PORTMAP__

#define BIT(n)          (1u << n)

#define RTS_PIN         5
#define TXD_PIN         6
#define CTS_PIN         7
#define RXD_PIN         8
#define LED1_PIN        17
#define LED2_PIN        18
#define LED3_PIN        19
#define LED4_PIN        20

constexpr uint32_t UART_RTS = BIT(RTS_PIN);
constexpr uint32_t UART_TXD = BIT(TXD_PIN);
constexpr uint32_t UART_CTS = BIT(CTS_PIN);
constexpr uint32_t UART_RXD = BIT(RXD_PIN);
constexpr uint32_t LED1     = BIT(LED1_PIN);
constexpr uint32_t LED2     = BIT(LED2_PIN);
constexpr uint32_t LED3     = BIT(LED3_PIN);
constexpr uint32_t LED4     = BIT(LED4_PIN);

#endif
