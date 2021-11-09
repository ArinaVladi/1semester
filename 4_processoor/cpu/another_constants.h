const int PROGRAM_CODE_MASK = 255;

enum PARAM_TYPES {
    LABEL = 1111,
    ALL   = 2222,
    NO    = 3333,
    _RET_ = 4444,
    _MOVE_ = 55555

};

enum ARG_TYPE {
    NOT_INIT = 0,
    CONST_   = 1,
    REG_     = 2,
    RAM      = 4,
    ERROR    = 8
};



#ifndef CPU_ANOTHER_CONSTANTS_H
#define CPU_ANOTHER_CONSTANTS_H

enum ARG_TYPE_MASKS {
CONST         = 32,
REG           = 64,
RAM_CONST     = 160,
RAM_REG       = 192,
RAM_REG_CONST = 224
};

enum {
    RAM_REG_CONST_ARG =  1,
    RAM_REG_ARG       =  2,
    RAM_CONST_ARG     =  3,
    REG_ARG           =  4,
    CONST_ARG         =  5,
    WRONG_ARG         = -1
};

#endif //CPU_ANOTHER_CONSTANTS_H
