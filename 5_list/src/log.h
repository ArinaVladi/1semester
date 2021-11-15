#pragma once

#include  <stdio.h>
#include <stdlib.h>
#include  <cassert>
#include <typeinfo>
#include  <cstring>

const char* const ELEMENT_NOT_SPECIFIED  = "was not provided";

#define ALWAYS_INLINE inline attribute((always_inline))
#define LOG_INIT_INFO this, __FILE__, __func__, __LINE__, "list", sizeof(double), "List", "double"

/* #define ASSERT_OK                                                                                      \
    if ((stackOK()) IF_ALWAYS_DUMP(|| 1)) {                                                            \
        int error = stackOK();                                                                         \
        dump_t theDump = {};                                                                           \
        theDump.initialize(this, LOCATION, "0", elem_size);                                            \
        theDump.print_dump_impl(this, print_dump_wrapper_void_to_stack, get_error_name(error), error); \
        IF_NOT_ALWAYS_DUMP(printf("Sth went wrong. Check log."));                                      \ */


typedef struct Dump {

    // core
    void* address = NULL;
    char* file;
    char* function;
    int   line;

    // optional
    const char* name;
    char* obj_type;
    char* elem_type;
    int elem_size;

    void clear_file ();

    void initialize (void* obj_address,                char* current_file,
                     const char* current_func,           int current_line,
                     const char* obj_name  =                          "0",
                     int current_elem_size =                         2038,
                     char* obj_type        = (char*)ELEMENT_NOT_SPECIFIED,
                     char* elem_type       = (char*)ELEMENT_NOT_SPECIFIED);

    void print_dump_impl (void* obj_address, void (*print_dump)(void* someObj), FILE* log_file);

} dump_t;

