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

#define UART_RTS        BIT(RTS_PIN)
#define UART_TXD        BIT(TXD_PIN)
#define UART_CTS        BIT(CTS_PIN)
#define UART_RXD        BIT(RXD_PIN)
#define LED1            BIT(LED1_PIN)
#define LED2            BIT(LED2_PIN)
#define LED3            BIT(LED3_PIN)
#define LED4            BIT(LED4_PIN)

#endif
