#ifndef INIT_H__
#define INIT_H__

#define MSK(value, shift)   (uint32_t)((value) << (shift))
#define BIT(n)              MSK(1, n)

#define LED1_PIN        17
#define LED2_PIN        18
#define LED3_PIN        19
#define LED4_PIN        20

#define LED1            BIT(LED1_PIN)
#define LED2            BIT(LED2_PIN)
#define LED3            BIT(LED3_PIN)
#define LED4            BIT(LED4_PIN)

#define LED_ALL         (LED1 | LED2 | LED3 | LED4)

void Init(void);

#endif
