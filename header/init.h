#ifndef INIT_H__
#define INIT_H__

#include "portmap.h"

#define MSK(value, shift)   (uint32_t)((value) << (shift))

#define LED_ALL         (LED1 | LED2 | LED3 | LED4)

#ifdef __cplusplus
    extern "C" void Init(void);
#else
    void Init(void);
#endif

#endif
