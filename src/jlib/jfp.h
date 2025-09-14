// Fixed point numbers

#include <stdint.h>



/// @brief Signed fixed point ( 1 bit signed, 10 int, 5 decimals)
/// This ranges from -16384 to +16384, with 1/32 resolution = 0.03125 in
/// decimals. This means the limit for the biggest number resulting out of
/// multiplication is 512, as the scaling will push it 512*32 = 16384
typedef int16_t SFP_10_5;
// The number one is represented by a shift to the right.
#define SFP_10_5_SHIFT 5
#define SFP_10_5_MSHIFT_X 3
#define SFP_10_5_MSHIFT_Y 2
#define SFP_10_5_ONE (1 << SFP_10_5_SHIFT)
#define FLOAT_TO_SFP_10_5(f_) ((SFP_10_5)(SFP_10_5_ONE * f_))

#define SFP_10_5_TO_FLOAT(sfp_) ((float)(sfp_) / SFP_10_5_ONE)
#define SFP_10_5_EPSILON 0.03125f
// DBG_NEAR_EQUAL is just intended for testing and debugging, and actually
// compares floats
#define SFP_10_5_DBG_NEAR_EQUAL(x_ , y_ , num_eps_ ) \
    ( x_ <  y_ + num_eps_* SFP_10_5_EPSILON  && \
      x_ >  y_ - num_eps_ * SFP_10_5_EPSILON  \
    )
// Regular NEAR_EQUAL is intended to compare two SFP_10_5, and just compare a
// multiple of Least Significant Bits
#define SFP_10_5_NEAR_EQUAL(x_, y_, num_LSB_) \
    ( x_ <  y_ + num_LSB_  && \
      x_ >  y_ - num_LSB  \
    )

// Mutltiplication can be implemented as a simple case, but one will run into
// issues with very larger and small numbers. By requiring the user to set how
// much values must be pre-shifted before multiplication, one can avoid losing
// precision. The post-shift will automatically ensure the shift is always equal
// to 5. Users should take care not to pre-shift more than a total of 5.
// Negative shifts are not desired, but at least gcc emits a warning if this happens.
#define SFP_10_5_MULT(x_, y_, x_pre_shift_, y_pre_shift_) \
    ( ( x_ >> x_pre_shift_ ) * ( y_  >> y_pre_shift_ ) \
     >> SFP_10_5_SHIFT - x_pre_shift_ - y_pre_shift_ )


// Mod 2 should be the same as doing x % (2<<SFP_10_5_SHIFT)
#define SFP_10_5_MOD_2(x_) (x_ % (2 << SFP_10_5_SHIFT) ) 

SFP_10_5 SFP_10_5_new_F(float);
SFP_10_5 SFP_10_5_new_D(double);
// SFP_10_5 SFP_10_5_mul(SFP_10_5, SFP_10_5);
// SFP_10_5 SFP_10_5_div(SFP_10_5, SFP_10_5);
// SFP_10_5 SFP_10_5_mod(SFP_10_5, SFP_10_5);

