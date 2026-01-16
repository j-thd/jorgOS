#include <stdint.h>

/// @brief Returns -1 if negative, 0 if zero, +1 if x is positive.
/// @param x - integer to determine sign of
/// @return [-1, 0, +1]
int8_t JM_sign(int32_t x){
    return (x > 0) - (x < 0);
}

/// @brief Addition that saturates at UINT8_MAX
/// @param x 
/// @param y 
/// @return 
uint8_t saturating_add_uint8_t(uint8_t x, uint8_t y){
    uint8_t sum = x + y;
    
    // If sum has overflown, it must be smaller than x or y
    if( sum < x ){
        return UINT8_MAX;
    }

    return sum;
}

/// @brief Addition (x-y) that saturates at 0
/// @param x - first operand
/// @param y  - second operand
/// @return  x - y 
uint8_t saturating_subtract_uint8_t(uint8_t x, uint8_t y){
    uint8_t diff = x - y;
    
    // If sum has underflown, it must be bigger than x
    if( diff > x ){
        return 0;
    }

    return diff;
}

/// @brief Addition that saturates at UINT8_MAX
/// @param x 
/// @param y 
/// @return 
uint32_t saturating_add_uint32_t(uint32_t x, uint32_t y){
    uint32_t sum = x + y;
    
    // If sum has overflown, it must be smaller than x or y
    if( sum < x ){
        return UINT32_MAX;
    }

    return sum;
}

/// @brief Addition (x-y) that saturates at 0
/// @param x - first operand
/// @param y  - second operand
/// @return  x - y 
uint32_t saturating_subtract_uint32_t(uint32_t x, uint32_t y){
    uint32_t diff = x - y;
    
    // If sum has underflown, it must be bigger than x
    if( diff > x ){
        return 0;
    }

    return diff;
}