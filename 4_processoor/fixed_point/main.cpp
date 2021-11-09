#include "fixed_point.h"

int main() {

    float num1 = 123.456;
    float num2 = 15.078;
    printf("float_to_fixed(%f) = %d\n", num1, float_to_fixed(num1));
    printf("fixed_to_float(%d) = %f\n", float_to_fixed(num2), fixed_to_float(float_to_fixed(num2)));

    num1 = 15.21;
    num2 = 12.29;
    printf("%f + %f = %f\n", num1, num2, fixed_to_float(fixed_add(float_to_fixed(num1), float_to_fixed(num2))));
    printf("%f + %f = %f\n", num1, num2, fixed_to_float(fixed_add(num1, num2)));

    num1 = 2.5;
    num2 = -2;
    printf("%f * (%f) = %f\n", num1, num2, fixed_to_float(fixed_mul(float_to_fixed(num1), float_to_fixed(num2))));
    printf("%f * (%f) = %f\n", num1, num2, fixed_to_float(fixed_mul(num1, num2)));

    num1 = -12.60;
    num2 = -2;
    printf("%f \\ (%f) = %f\n", num1, num2, fixed_to_float(fixed_div(float_to_fixed(num1), float_to_fixed(num2))));
    printf("%f \\ (%f) = %f\n", num1, num2, fixed_to_float(fixed_div(num1, num2)));

    num1 = 12.60;
    num2 = 112;
    printf("%f - (%f) = %f\n", num1, num2, fixed_to_float(fixed_sub(float_to_fixed(num1), float_to_fixed(num2))));
    printf("%f - (%f) = %f\n", num1, num2, fixed_to_float(fixed_sub(num1, num2)));

    num1 = 123;
    printf("sqrt(%f) = %f\n", num1, fixed_to_float(fixed_sqrt(float_to_fixed(num1))));
}

