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
#define SFP_10_5_MULT(x_, y_) \
    ( ( x_ >> SFP_10_5_MSHIFT_X ) * ( y_  >> SFP_10_5_MSHIFT_Y ) )
#define SFP_10_5_TO_FLOAT(sfp_) ((float)(sfp_) / SFP_10_5_one)

SFP_10_5 SFP_10_5_new_F(float);
SFP_10_5 SFP_10_5_new_D(double);
// SFP_10_5 SFP_10_5_mul(SFP_10_5, SFP_10_5);
// SFP_10_5 SFP_10_5_div(SFP_10_5, SFP_10_5);
// SFP_10_5 SFP_10_5_mod(SFP_10_5, SFP_10_5);

