//
// Created by ArinaVladi on 20.09.2021.
//

#ifndef ONEGIN_ONE_FILE_MY_UTILITIES_H
#define ONEGIN_ONE_FILE_MY_UTILITIES_H

#include <cstring>
#include <cassert>
#include <ctype.h>

#include "text.h"



#define set_mask(start, end, mask) for (int i = start; i <= end; i++) Symbols[i] = Symbols[i] | mask;

/*enum {  IsAlpha        =  1,
        IsDigit        =  2,
        IsRomanDigit   =  4,
        IsNotLineBegin =  8,
        IsPunct        = 16,
        IsStrangeDigit = 32,
        IsSpace        = 64,
        IsUpper        = 128,
        IsLower        = 256
};*/

void my_qsort(void *, int, size_t, int (*)(const void *, const void *));
int my_cmp_2(void const *v1, void const *v2, int direction);
void my_swap(void *, void *, size_t);

int isUnwantedDigit(char);
int isRomanDigit(char);

void set_masks ();
/*int  compare (unsigned char c, int mask);*/

#endif //ONEGIN_ONE_FILE_MY_UTILITIES_H
