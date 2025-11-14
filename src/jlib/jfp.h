// Fixed point numbers

#include <stdint.h>



// SFP_10_5

/// @brief Signed fixed point (10,5) ( 1 bit signed, 10 int, 5 decimals)
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
#define SFP_10_5_TO_INT(sfp_) ((int)(sfp_) / SFP_10_5_ONE)
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


// SFP_5_10
/// @brief Signed fixed point (5,10) ( 1 bit signed, 10 int, 5 decimals)
/// This ranges from -16 to +32, with 1/(2^10) resolution = 0.0009765625f in
/// decimals. This means the limit for the biggest number resulting out of
/// multiplication is 512, as the scaling will push it 512*32 = 16384
typedef int16_t SFP_5_10;
// The number one is represented by a shift to the right.
#define SFP_5_10_SHIFT 10
#define SFP_5_10_MSHIFT_X 5
#define SFP_5_10_MSHIFT_Y 5
#define SFP_5_10_ONE (1 << SFP_5_10_SHIFT)
#define FLOAT_TO_SFP_5_10(f_) ((SFP_5_10)(SFP_5_10_ONE * f_))

#define SFP_5_10_TO_FLOAT(sfp_) ((float)(sfp_) / SFP_5_10_ONE)
#define SFP_5_10_TO_INT(sfp_) ((int)(sfp_) / SFP_5_10_ONE)
#define SFP_5_10_EPSILON 0.0009765625f
// DBG_NEAR_EQUAL is just intended for testing and debugging, and actually
// compares floats
#define SFP_5_10_DBG_NEAR_EQUAL(x_ , y_ , num_eps_ ) \
    ( x_ <  y_ + num_eps_* SFP_5_10_EPSILON  && \
      x_ >  y_ - num_eps_ * SFP_5_10_EPSILON  \
    )

#define SFP_5_10_NEAR_EQUAL(x_, y_, num_LSB_) \
    ( x_ <  y_ + num_LSB_  && \
      x_ >  y_ - num_LSB  \
    )


#define SFP_5_10_MULT(x_, y_, x_pre_shift_, y_pre_shift_) \
    ( ( x_ >> x_pre_shift_ ) * ( y_  >> y_pre_shift_ ) \
     >> SFP_5_10_SHIFT - x_pre_shift_ - y_pre_shift_ )


// Mod 2 should be the same as doing x % (2<<SFP_10_5_SHIFT)
#define SFP_5_10_MOD_2(x_) (x_ % (2 << SFP_5_10_SHIFT) )

SFP_5_10 SFP_5_10_new_F(float);
SFP_5_10 SFP_5_10_new_D(double);


/// I went a bit too deep in the SFP hole, and learned enough. Let's cut myself
/// free and get the range correctly on the HSL_to_RGB function correctly by
/// just going to 32-bit ints for SFP calculations.
/// Later, if I feel like doing something, I can automate the creation of this
/// file to create all the types I need and even define conversion macros
/// between them.

// SFP_11_20
/// @brief Signed fixed point (5,10) ( 1 bit signed, 10 int, 5 decimals)
/// This ranges from -16 to +32, with 1/(2^10) resolution = 0.0009765625f in
/// decimals. This means the limit for the biggest number resulting out of
/// multiplication is 512, as the scaling will push it 512*32 = 16384
typedef int32_t SFP_11_20;
// The number one is represented by a shift to the right.
#define SFP_11_20_SHIFT 20
#define SFP_11_20_MSHIFT_X 10
#define SFP_11_20_MSHIFT_Y 10
#define SFP_11_20_ONE (1 << SFP_5_10_SHIFT)
#define FLOAT_TO_SFP_11_20(f_) ((SFP_11_20)(SFP_11_20_ONE * f_))

#define SFP_11_20_TO_FLOAT(sfp_) ((float)(sfp_) / SFP_11_20_ONE)
#define SFP_11_20_TO_INT(sfp_) ((int)(sfp_) / SFP_11_20_ONE)
#define SFP_11_20_EPSILON 0,00000095367431640625f
// DBG_NEAR_EQUAL is just intended for testing and debugging, and actually
// compares floats
#define SFP_11_20_DBG_NEAR_EQUAL(x_ , y_ , num_eps_ ) \
    ( x_ <  y_ + num_eps_* SFP_11_20_EPSILON  && \
      x_ >  y_ - num_eps_ * SFP_11_20_EPSILON  \
    )

#define SFP_11_20_NEAR_EQUAL(x_, y_, num_LSB_) \
    ( x_ <  y_ + num_LSB_  && \
      x_ >  y_ - num_LSB  \
    )


#define SFP_11_20_MULT(x_, y_, x_pre_shift_, y_pre_shift_) \
    ( ( x_ >> x_pre_shift_ ) * ( y_  >> y_pre_shift_ ) \
     >> SFP_11_20_SHIFT - x_pre_shift_ - y_pre_shift_ )


// Mod 2 should be the same as doing x % (2<<SFP_10_5_SHIFT)
#define SFP_11_20_MOD_2(x_) (x_ % (2 << SFP_11_20_SHIFT) )

SFP_11_20 SFP_11_20_new_F(float);
SFP_11_20 SFP_11_20_new_D(double);