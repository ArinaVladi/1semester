#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <cstdint>
#include <cstring>

// #include "another_constants.h"

typedef struct Listing {
    
    FILE* lst_file;

    void open_and_init_file(char* name);
    void print_file_header(char* header, int version);
    void document_command(int adress, char* name, int num, int arg_num, int param_type, char* program, int cmd);
    // void listing_t::get_arg_type(int cmd);

} listing_t;