#include "printUtils.hpp"

#include "../lib/console.h"
#include "../lib/hw.h"

uint64 lockPrint = 0;

// #define LOCK() while(copy_and_swap(lockPrint, 0, 1));
// #define UNLOCK() while(copy_and_swap(lockPrint, 1, 0));

void printString(char* string) {
    // LOCK();

    while(*string != '\0') {
        __putc(*string);
        string++;
    }

    // UNLOCK();
}

char *getString(char *buf, int max) {
    // LOCK();
    int i, cc;
    char c;

    for (i = 0; i + 1 < max; ) {
        cc = __getc();

        if (cc < 1) break;

        c = cc;

        buf[i++] = c;

        if (c == '\n' || c == '\r') break;
    }

    buf[i] = '\0';

    // UNLOCK();

    return buf;
}

int stringToInt(const char *string) {
    int n = 0;

    while ('0' <= *string && *string <= '9') {
        n = n * 10 + *string++ - '0';
    }

    return n;
}

char digits[] = "0123456789ABCDEF";

void printInt(uint64 xx, int base, int sgn) {
    // LOCK();

    char buf[16];
    int i, neg = 0;
    uint x;

    if (sgn && xx < 0) {
        neg = 1;
        x = -xx;
    } else
        x = xx;

    i = 0;

    do {
        buf[i++] = digits[x % base];
    } while ((x /= base) != 0);

    if (neg) buf[i++] = '-';

    while (--i >= 0) {
        __putc(buf[i]);
    }

    // UNLOCK();
}
