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

static constexpr size_t WRITE_BUFFER_SIZE = 100;
static constexpr char UNSENT = ~0;
static char LENGTH_ERROR_STR[] = "Error: Call to _write used more than 1 character.";
static constexpr size_t LENGTH_ERROR_LEN = sizeof(LENGTH_ERROR_STR) - 1;

static char HEAP_OVERFLOW_STR[] = "Error: Heap needs to expand into the stack.";
static constexpr size_t HEAP_OVERFLOW_LEN = sizeof(HEAP_OVERFLOW_STR) - 1;

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
    static char *heap_end = &__end__;
    char *prev_heap_end;

    prev_heap_end = heap_end;
    if (heap_end + incr > (char *)__get_MSP()) {
        SerialWrite(HEAP_OVERFLOW_STR, HEAP_OVERFLOW_LEN);
        _exit();
    }

    heap_end += incr;
    return (caddr_t)prev_heap_end;
}

int _write(int, char *ptr, int len) {
    static char buffer[WRITE_BUFFER_SIZE] = { UNSENT };
    static size_t length = 0;

    if (len != 1) {
        SerialWrite(LENGTH_ERROR_STR, LENGTH_ERROR_LEN);
        _exit();
    }
    if (buffer[length] == SERIAL_UNRELEASED) {
        errno = ENOSPC;
        return -1;
    }
    if (buffer[length] == SERIAL_RELEASED)
        length = 0;

    buffer[length] = *ptr;
    length++;

    if (*ptr == '\n') {
        buffer[length] = '\r';
        length++;
    }

    if (*ptr == '\n' || length + 2 >= WRITE_BUFFER_SIZE) {
        buffer[length] = SERIAL_UNRELEASED;

        if (SerialWrite(buffer, length) != SERIAL_SUCCESS) {
            errno = ENOSPC;
            return -1;
        }
    }
    else
        buffer[length] = UNSENT;

    return 1;
}
