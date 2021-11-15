#pragma once

#include <stdio.h>
#include <cassert>
#include <cstring>
#include <stdlib.h>
#include <cmath>

#include "log.h"
#include "constants.h"

#define ASSERT_OK                                                                                         \
    log_file = fopen(log_file_name, "a");                                                                 \
    {dump_t theDump = {};                                                                                  \
    theDump.initialize(LOG_INIT_INFO);                                                                    \                             
    theDump.print_dump_impl(this, print_log_wrappaer, log_file); }                                         \
    fclose(log_file);                                            

enum class RETURNED_SIGNALS {
    
    OK                    = 0,
    CTOR_FAILURE          = 1,
    INCREASE_FAILURE      = 2,
    WRONG_IS_SORTED_STATE = 3,
    DECREASE_FAILURE      = 4,
    WRONG_INDEX           = 5,
    SORT_FAILURE          = 6,
    INDEX_FUNC_FAILURE    = 7,
};

enum class ERRORS {
    OK                 =  1,
    ZERO_ELEMENT_ERROR =  2,
    NEXT_ERROR         =  4,
    PREV_ERROR         =  8,
    SIZE_ERROR         = 16,
};

typedef double elem_t;

typedef struct {

    elem_t data;
    int    next;
    int    prev;
} Node;

typedef struct {

    Node*      elements;
    int        capacity;
    int            head;
    int            tail;
    int            size;
    int      first_free;
    bool      is_sorted;
    int          status; 

    char* log_file_name;
    FILE*      log_file;

    RETURNED_SIGNALS Ctor();
    RETURNED_SIGNALS Dtor();

    elem_t           front();
    elem_t           back(); 

    RETURNED_SIGNALS increase_capacity(); 
    RETURNED_SIGNALS decrease_capacity(); 

    RETURNED_SIGNALS insert(size_t index, const elem_t* value); 
    RETURNED_SIGNALS insert_by_logical_index (size_t logical_index, const elem_t* val);
    RETURNED_SIGNALS push_front(const elem_t* value); 
    RETURNED_SIGNALS push_back(const elem_t* value); 
    
    RETURNED_SIGNALS erase(size_t index);                                         
    RETURNED_SIGNALS erase_by_logical_index(size_t logical_index);
    RETURNED_SIGNALS pop_front();                                                   
    RETURNED_SIGNALS pop_back();

    RETURNED_SIGNALS sort();

    RETURNED_SIGNALS get_physical_index_by_logical_index(size_t pos, size_t* res);
    RETURNED_SIGNALS data_by_logical_index(size_t index, elem_t* val);
    RETURNED_SIGNALS data_by_physical_index(size_t index, elem_t* val);

    int              verify();
    ERRORS           verify_zero_element();
    ERRORS           verify_head_to_tail();
    ERRORS           verify_tail_to_head();
    ERRORS           verify_size();

    void             do_log();
    int              print_error_message_to_log_file(int error);
    void             compare_error_code_with_mask(int error, ERRORS mask, int* error_name_index);

    void             dump_list(int test_num, int pass);
    void             dump_table(int test_num, int pass);
    void             do_html(int num_of_tests);
} List;

void print_log_wrappaer (void* someObj);