#ifndef UARTE_H__
#define UARTE_H__

#include <cstddef>

constexpr int SERIAL_SUCCESS     =  0;
constexpr int SERIAL_BUFFER_FULL = -1;

void UartInit();
int SerialWrite(const char *str, size_t length);

#endif
