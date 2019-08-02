#ifndef UARTE_H__
#define UARTE_H__

#include <cstddef>

constexpr int SERIAL_SUCCESS     =  0;
constexpr int SERIAL_BUFFER_FULL = -1;
constexpr int8_t SERIAL_UNRELEASED =  0;
constexpr int8_t SERIAL_RELEASED   =  1;

void UartInit();
int SerialWrite(const char *str, size_t length, int8_t *released = nullptr);

#endif
