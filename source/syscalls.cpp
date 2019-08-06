#include <nrf.h>

extern "C" void _exit();

static constexpr uint32_t TURN_OFF =
        POWER_SYSTEMOFF_SYSTEMOFF_Enter << POWER_SYSTEMOFF_SYSTEMOFF_Pos;

void _exit() {
    while (true)
        NRF_POWER->SYSTEMOFF = TURN_OFF;
}
