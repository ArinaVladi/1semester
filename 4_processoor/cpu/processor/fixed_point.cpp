#include "fixed_point.h"

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <cassert>

//const int    fractional_bits =    16;
//const int    fixed_type_bits =    32;
//
//typedef int32_t fixed_type;
//typedef int64_t expand_type;

fixed_type float_to_fixed(float inp);
float fixed_to_float(fixed_type inp);
fixed_type float_to_fixed(int inp);

fixed_type float_to_fixed(float inp) {
    return (fixed_type)(inp * (1 << fractional_bits));
}
fixed_type float_to_fixed(fixed_type inp) {
    return inp;
}



float fixed_to_float(fixed_type inp) {
    return ((float)inp) / (1 << fractional_bits);
}

#define OP1(name, code)                                                                 \
        fixed_type fixed_ ## name (fixed_type inp) code
       /* fixed_type fixed_ ## name (float inp) {                                         \
            return fixed_ ## name (float_to_fixed(inp));                                \
        }*/

#define OP2(name, code)                                                                 \
        fixed_type fixed_ ## name (fixed_type inp_1, fixed_type inp_2) code             \
        fixed_type fixed_ ## name (float inp_1, float inp_2) {                          \
            return fixed_ ## name (float_to_fixed(inp_1), float_to_fixed(inp_2));       \
        }

    #include "fixed_def.h"

#undef OP1
#undef OP2