#include <cerrno>
#include <nrf.h>
#include <sys/stat.h>
#include "uarte.h"

#undef errno
extern int errno;

extern "C" void _exit();
extern "C" int _close(int file);
extern "C" int _fstat(int file, struct stat *st);
extern "C" int _getpid();
extern "C" int _isatty(int file);
extern "C" int _kill(int pid, int sig);
extern "C" int _lseek(int file, int ptr, int dir);
extern "C" int _read(int file, char *ptr, int len);
extern "C" caddr_t _sbrk(int incr);
extern "C" int _write(int file, char *ptr, int len);

static void ErrorExit(const char str[], size_t len) __attribute((noreturn));

static constexpr char HEAP_OVERFLOW_STR[] = "Error: Heap needs to expand beyond limit.\n\r";

static constexpr int8_t UNSENT = -1;
static constexpr char LENGTH_ERROR_STR[] = "Error: Call to _write used more than 1 character.\n\r";

static constexpr size_t BUFFER_SIZE = 100;
static char buffer[BUFFER_SIZE];

void _exit() {
    while (true);
}

int _close(int) {
    return -1;
}

int _fstat(int, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _getpid() {
    return 1;
}

int _isatty(int) {
    return 1;
}

int _kill(int, int) {
    errno = EINVAL;
    return -1;
}

int _lseek(int, int, int) {
    return 0;
}

int _read(int, char *, int) {
    return 0;
}

caddr_t _sbrk(int incr) {
    extern char __end__;
    extern char __HeapLimit;
    static char *heap_end = &__end__;
    char *prev_heap_end;

    prev_heap_end = heap_end;
    if (heap_end + incr > &__HeapLimit)
        ErrorExit(HEAP_OVERFLOW_STR, StringLen(HEAP_OVERFLOW_STR));

    heap_end += incr;
    return (caddr_t)prev_heap_end;
}

int _write(int, char *ptr, int len) {
    static int8_t released = UNSENT;
    static size_t length = 0;

    if (len != 1)
        ErrorExit(LENGTH_ERROR_STR, StringLen(LENGTH_ERROR_STR));
    if (released == SERIAL_UNRELEASED) {
        errno = ENOSPC;
        return -1;
    }
    if (released == SERIAL_RELEASED)
        length = 0;

    buffer[length++] = *ptr;
    if (*ptr == '\n')
        buffer[length++] = '\r';

    if (*ptr == '\n' || length + 1 >= BUFFER_SIZE) {
        if (SerialWrite(buffer, length, &released) != SERIAL_SUCCESS) {
            errno = ENOSPC;
            return -1;
        }
    }

    return 1;
}

void ErrorExit(const char str[], size_t len) {
    extern char __data_start__;

    if (str < &__data_start__) {
        for (size_t i = 0; i < BUFFER_SIZE && i < len; i++, str++)
            buffer[i] = *str;

        str = buffer;
    }

    SerialWrite(str, len);
    _exit();
    __builtin_unreachable();
}
