#include "../h/console.hpp"
#include "../lib/console.h"

Console* Console::instance = nullptr;

Console * Console::getInstance() {
    if (instance == nullptr) {
        instance = new Console();
    }

    return instance;
}

char Console::getc() {
    return __getc();
}

void Console::putc(char c) {
    __putc(c);
}