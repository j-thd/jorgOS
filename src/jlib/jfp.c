#include "jfp.h"


SFP_10_5 SFP_10_5_new_F(float f){
    return (SFP_10_5)(SFP_10_5_ONE * f);
}
SFP_10_5 SFP_10_5_new_D(double d){
    return (SFP_10_5)(SFP_10_5_ONE * d);
};