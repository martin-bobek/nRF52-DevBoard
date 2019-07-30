#ifndef LOCK_H__
#define LOCK_H__

#include <nrf.h>

class Mutex {
public:
    inline bool Lock();
    void Unlock() { locked = false; }
private:
     uint8_t locked = false;
};

bool Mutex::Lock() {
    while (!__LDREXB(&locked))
        if (!__STREXB(true, &locked))
            return true;

    return false;
}

#endif
