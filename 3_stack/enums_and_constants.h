#ifndef PRIMITIVE_STACK_ENUMS_AND_CONSTANTS_H
#define PRIMITIVE_STACK_ENUMS_AND_CONSTANTS_H

inline const unsigned int MAX_ERROR_POW  =                  8; //! если совпадает наименование - то код гарантированно включается один раз
const char* const ELEMENT_NOT_SPECIFIED  = "was not provided";

inline const char* ERROR_NAMES[] = {  //! c++17

        "stack ptr is NULL\n",                         // 0
        "stack data ptr is null\n",                    // 1
        "capacity os negative\n",                      // 2
        "size is negative\n",                          // 3
        "size > capacity\n",                           // 4
        "left canary is destroyed\n",                  // 5
        "right canary is destroyed\n",                 // 6
        "data hash values do not match\n",             // 7
        "struct hash values do not match\n"            // 8
};

enum ERRORS {

        NULL_STACK_PTR           =   1,                // 0
        NULL_STACK_DATA_PTR      =   2,                // 1
        NEGATIVE_CAPACITY        =   4,                // 2
        NEGATIVE_SIZE            =   8,                // 3
        OVERFLOW                 =  16,                // 4
        CANARY_L_DESTROYED       =  32,                // 5
        CANARY_R_DESTROYED       =  64,                // 6
        DATA_HASH_DONT_MATCH     = 128,                // 7
        STRUCT_HASH_DONT_MATCH   = 256,                // 8
};

enum status {

        CTOR_ERROR               =   1,                // 0
        PUSH_ERROR               =   2,                // 1
        POP_ERROR                =   4,                // 2

        STACK_SHRINK_ERROR       =   8,                // 3
        STACK_INCREASE_ERROR     =  16,                // 4

        NEGATIVE_ELEM_SIZE       =  32,                // 5
        MEMORY_ALLOCATION_ERROR  =  64,                // 6
        STACK_IS_EMPTY           = 128,                // 7

        STACK_OK                 =   0                 // 8
};

inline const char* METHOD_ERROR_NAMES[] {

        "Ctor -> ",                                    // 0
        "Push -> ",                                    // 1
        "Pop -> ",                                     // 2

        "Shrink -> ",                                  // 3
        "Increase -> ",                                // 4

        "negative element size",                       // 5
        "memory was not allocated",                    // 6
        "stack is empty"                               // 7
};

#endif //PRIMITIVE_STACK_ENUMS_AND_CONSTANTS_H
