#pragma once

#include "list.h"

enum class FUNC_ARG_TYPES {
    INDEX = 1,
    ELEM  = 2,
    BOTH  = 3,
    NO    = 4,
};

typedef union {
    RETURNED_SIGNALS (List::*both_func_ptr) (size_t index, const elem_t*);
    RETURNED_SIGNALS (List::*elem_func_ptr) (const elem_t*);
    RETURNED_SIGNALS (List::*index_func_ptr) (size_t index);
    RETURNED_SIGNALS (List::*no_func_ptr) ();
} any_list_func_ptr;

RETURNED_SIGNALS  func_wrapper(char*        test_name,
                               List*             list, 
                               any_list_func_ptr func, 
                               FUNC_ARG_TYPES    type,        
                               int              index = -13,
                               const elem_t* elem_ptr = (elem_t*)13);

void do_html(int num_of_tests);


