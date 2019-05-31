#include <nrf.h>

#define BIT(n) (uint32_t)(1u << n)

int main(void) {
    NRF_P0->DIRSET = BIT(17) | BIT(18) | BIT(19) | BIT(20);
}
