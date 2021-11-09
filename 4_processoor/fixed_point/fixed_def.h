#ifndef FIXED_POINT_FIXED_DEF_H
#define FIXED_POINT_FIXED_DEF_H

OP1(get_sign, {
    return (inp >> (sizeof(fixed_type) - 1));
})

OP1(change_sign, {
    return ~inp+1;
})

// https://github.com/chmike/fpsqrt/blob/master/fpsqrt.c
OP1(sqrt, {

    assert((inp >= 0) && "sqrt argument < 0!");

    uint32_t   t =          0;
    uint32_t   q =          0;
    fixed_type b = 0x40000000;
    fixed_type r =        inp;

    while (b > 0x40) {
        t = q + b;
        if (r >= t) {
            r -= t;
            q = t + b; // equivalent to q += 2*b
        }
        r <<= 1;
        b >>= 1;
    }
    q >>= 8;
    return q;
})


OP2 (add, {
    if (inp_1 > 0) {
        if (inp_2 > INT32_MAX - inp_1) {
            inp_2 = INT32_MAX - inp_1;
        }
    }
    else if (inp_2 < INT32_MIN - inp_1) {
        inp_2 = INT32_MIN - inp_1;
    }

    return inp_1 + inp_2;
})


OP2(mul, {
    fixed_type sign1 = fixed_get_sign(inp_1);
    fixed_type sign2 = fixed_get_sign(inp_2);

    if (sign1 == sign2) {
        if (inp_2 > (expand_type)(INT32_MAX * inp_2)) {
            return INT32_MAX; // inp_2 = inp_1 / INT32_MAX
        }
    }
    else if (abs(inp_2) > abs(((expand_type)(INT32_MAX * inp_1)))) {
        return INT32_MIN;
    }
    return ((expand_type)inp_1 * (expand_type)inp_2) / (1 << 16);
})

OP2(div, {
    fixed_type sign1 = fixed_get_sign(inp_1);
    fixed_type sign2 = fixed_get_sign(inp_2);

    assert((inp_2 != 0) && "Divisor = 0");

    if (sign1 == sign2) {
        if (inp_2 > (expand_type)(INT32_MAX * inp_1)) {
            return INT32_MAX; // inp_2 = inp_1 / INT32_MAX
        }
    }
    else if (abs(inp_2) > abs((expand_type)(INT32_MAX * inp_1))) {
        return INT32_MIN;
    }
    return ((expand_type)inp_1 * (1 << 16)) / inp_2;
})

OP2(sub, {
    fixed_add(inp_1, fixed_change_sign(inp_2));
})



#endif //FIXED_POINT_FIXED_DEF_H
