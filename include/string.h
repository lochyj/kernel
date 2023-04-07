#pragma once

#include <stdint.h>

void* memcpy(void* dest, const void* src, int count);
void* memset(void* dest, unsigned char val, int count);
unsigned short* memsetw(unsigned short* dest, unsigned short val, int count);
int strcmp(const char *s1, const char *s2);
size_t strlen(const char *str);
