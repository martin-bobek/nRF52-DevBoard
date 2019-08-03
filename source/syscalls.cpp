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
