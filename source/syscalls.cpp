#include <cerrno>
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

static void ErrorExit(const char str[], size_t len) __attribute((noreturn));

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
