#include <math.h>

int abs(int x) {
    uint32_t temp = x >> 31;     // make a mask of the sign bit
    x ^= temp;                   // toggle the bits if value is negative
    x += temp & 1;               // add one if value was negative
    return x;
}
