#ifndef STACK_TYPEDEF_DUMP_H
#define STACK_TYPEDEF_DUMP_H

#include  <stdio.h>
#include <stdlib.h>
#include  <cassert>
#include <typeinfo>
#include  <cstring>


#include          "D:\MainFolder\repos\computer3\cpu\macro_util.h"
#include "D:\MainFolder\repos\computer3\cpu\enums_and_constants.h"

void method_error          (int status);
void print_method_error (char* message);

//----------------------------------------struct Dump----------------------------------------

typedef struct Dump {

    // to work with file
    static char* file_name;
    FILE* log_file;

    // core
    void* address = NULL;
    char*           file;
    char* function;
    int line;

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

    void print_dump_impl (void* obj_address, void (*print_dump)(void* someStack), char* error_message = "0", int error = 2038);

} dump_t;

#endif //STACK_TYPEDEF_DUMP_H

