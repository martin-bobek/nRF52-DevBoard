#include <sys/stat.h>

extern "C" void _exit();
extern "C" int _close(int file);
extern "C" int _fstat(int file, struct stat *st);

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
