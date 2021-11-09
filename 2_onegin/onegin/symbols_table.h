//
// Created by ArinaVladi on 22.09.2021.
//

#ifndef ONEGIN_ONE_FILE_SYMBOLS_TABLE_H
#define ONEGIN_ONE_FILE_SYMBOLS_TABLE_H

enum {  IsAlpha        =  1,
        IsDigit        =  2,
        IsRomanDigit   =  4,
        IsNotLineBegin =  8,
        IsPunct        = 16,
        IsStrangeDigit = 32,
        IsSpace        = 64,
        IsUpper        = 128,
        IsLower        = 256
};

int  compare (unsigned char c, int mask);

#endif //ONEGIN_ONE_FILE_SYMBOLS_TABLE_H
