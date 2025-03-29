#ifndef PRINTUTILS_HPP
#define PRINTUTILS_HPP

typedef unsigned long uint64;

extern "C" uint64 copy_and_swap(uint64 &lock, uint64 expected, uint64 desired);

void printString(char* string);

char* getString(char* buf, int max);

int stringToInt(const char* string);

void printInt(int xx, int base = 10, int sgn = 0);

#endif //PRINTUTILS_HPP