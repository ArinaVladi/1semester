#ifndef FIXED_POINT_FIXED_POINT_H
#define FIXED_POINT_FIXED_POINT_H

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <cassert>

const int    fractional_bits =    16;
const int    fixed_type_bits =    32;

typedef int32_t fixed_type;
typedef int64_t expand_type;

fixed_type float_to_fixed(float inp);
float fixed_to_float(fixed_type inp);

#define OP1(name, code)                                                                 \
fixed_type fixed_ ## name (fixed_type inp);                                             \
fixed_type fixed_ ## name (float inp);

#define OP2(name, code)                                                                 \
fixed_type fixed_ ## name (fixed_type inp_1, fixed_type inp_2);                         \
fixed_type fixed_ ## name (float inp_1, float inp_2);

#include "fixed_def.h"

#undef OP1
#undef OP2

/*fixed_type fixed_mul(fixed_type inp_1, fixed_type inp_2);
fixed_type fixed_mul(float inp_1, float inp_2);
fixed_type fixed_add(fixed_type inp_1, fixed_type inp_2);
fixed_type fixed_add(float inp_1, float inp_2);
fixed_type fixed_div(fixed_type inp_1, fixed_type inp_2);
fixed_type fixed_div(float inp_1, float inp_2);
fixed_type fixed_sub(fixed_type inp_1, fixed_type inp_2);
fixed_type fixed_sub(float inp_1, float inp_2);
fixed_type fixed_sqrt (fixed_type value);*/

#endif //FIXED_POINT_FIXED_POINT_H
