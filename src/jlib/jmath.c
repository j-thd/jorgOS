#include <stdint.h>

/// @brief Returns -1 if negative, 0 if zero, +1 if x is positive.
/// @param x - integer to determine sign of
/// @return [-1, 0, +1]
int8_t JM_sign(int32_t x){
    return (x > 0) - (x < 0);
}