#include <cctype>
#include <cerrno>
#include "uarte.h"

extern "C" void _exit();
extern "C" int _write(int file, char *ptr, int len);

constexpr size_t WRITE_BUFFER_SIZE = 40;

void _exit() {
    while (true);
}

int _write(int, char *ptr, int len) {
    static char buffer[WRITE_BUFFER_SIZE];
    static size_t length = 0;

    if (buffer[length] != SERIAL_RELEASED) {
        errno = ENOSPC;
        return -1;
    }

    size_t index;
    for (index = 0; index < (size_t)len && index + 1 < WRITE_BUFFER_SIZE; index++)
        buffer[index] = ptr[index];
    buffer[index] = SERIAL_UNRELEASED;

    if (SerialWrite(buffer, index) == SERIAL_SUCCESS)
        return index;

    errno = ENOSPC;
    return -1;
}
