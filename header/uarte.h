#ifndef UARTE_H__
#define UARTE_H__

constexpr int SERIAL_SUCCESS     =  0;
constexpr int SERIAL_BUFFER_FULL = -1;

void UartInit();
int SerialWrite(const char *str);

#endif
