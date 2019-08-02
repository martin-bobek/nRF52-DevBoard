extern "C" void _exit();
extern "C" int _close(int file);

void _exit() {
    while (true);
}

int _close(int) {
    return -1;
}
