#ifndef STACK_EDIT_STACK_H
#define STACK_EDIT_STACK_H

#include  <stdio.h>
#include <stdlib.h>
#include  <cassert>
#include  <cstring>
#include  <cstdint>
#include <cxxabi.h>
#include   <time.h>

#include "enums_and_constants.h"
#include          "macro_util.h"

const int INT_POISON       = 2038;
const double DOUBLE_POISON = 2038.00;
const int INIT_CAPACITY    =    5;
const int MULTIPLIER       =    2;

#define LOCATION __FILE__, __func__, __LINE__

typedef uint64_t canary_t;

//-------------------------------------additional methods-------------------------------------

char* get_error_name (int error_code);
void print_dump_wrapper_void_to_stack (void* someStack);

//----------------------------------------struct stack_t----------------------------------------

typedef struct Stack {

    IF_CANARY_PROTECTION (canary_t canary_l);

    void*        data;
    int      capacity;
    int          size;
    int     elem_size;

    #ifdef HASH_PROTECTION
        uint64_t   data_hash;
        uint64_t struct_hash;
    #endif // HASH_PROTECTION

    IF_CANARY_PROTECTION (canary_t canary_r);

    // public interface
    int      Ctor (int elem_type);
    void     Dtor ();

    int      push (const void* value);
    int      pop ();
    void     top (void* target);

    // size management methods
    int      shrink_stack ();
    int      increase ();

    // protection methods
    IF_HASH_PROTECTION (uint32_t get_hash (void* some_data, int size));
    void     get_hash_wrapper ();
    void     print_stack_dump (int error);
    IF_VERIFIER(int stackOK ());


} stack_t;

#endif //STACK_EDIT_STACK_H
