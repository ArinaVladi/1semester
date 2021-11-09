#include "listing.h"



enum ARG_TYPE_MASKS {

    LST_CONST         = 32,
    LST_REG           = 64,
    LST_RAM_CONST     = 160,
    LST_RAM_REG       = 192,
    LST_RAM_REG_CONST = 224
};

#define IS(x, mask) ((x & mask) == mask)

// always returns address
#define GET_ARG(cmd)                                                                                                   \
    IS(cmd, LST_RAM_REG_CONST)?          sizeof(char) :                                                                \
        (IS(cmd, LST_RAM_REG)?           sizeof(char) :                                                                \
            (IS(cmd, LST_RAM_CONST)?     sizeof(int)  :                                                                \
                (IS(cmd, LST_REG)?       sizeof(char) :                                                                \
                    (IS(cmd, LST_CONST)? sizeof(int)  :                                                                \
                                         NULL))))


const int PROGRAM_CODE_MASK = 255;


void listing_t::open_and_init_file(char* name) {
    
    lst_file = fopen(name, "w");
    assert(lst_file != nullptr);
    
    fprintf(lst_file, "%s:  %s, %s\n", "Listing", __DATE__, __TIME__);
}

void listing_t::print_file_header(char* header, int version) {
    fprintf(lst_file, "Bin file header: signature = %s, version = %d\n\n", header, version);
    fprintf(lst_file, "|    ip    |         binary code          | command code | command name |  argument_1  |  argument_2  |\n"
                      "|----------|------------------------------|--------------|--------------|--------------|--------------|\n");
}

void listing_t::document_command(int adress, char* name, int num, int arg_num, int param_type, char* program, int cmd) {
    
   

    int arg_1 = 111;
    int arg_2 = 222;
    void* arg1;
    fprintf(lst_file, "|  %#06x  |         binary code          |%14d|%14s", adress, num, name);
    if (arg_num == 0) {
        fprintf(lst_file, "|      --      |      --      |\n");
    }
    else if (arg_num == 1) {

        int arg_size = GET_ARG(cmd);

        if (arg_size == 0) {
            arg1 = (void*) calloc(1, arg_size);
            memcpy(arg1, program + adress, arg_size);
            fprintf(lst_file, "| label (0x%2hhx) |      --      |\n", arg1);
        }
        else {
            arg1 = (void*) calloc(1, arg_size);
            memcpy(arg1, program + adress, arg_size);
            if (arg_size == sizeof(float)) fprintf(lst_file, "|%14f|      --      |\n", *(float*)arg1);
            else                           fprintf(lst_file, "|    reg%3d    |      --      |\n", *(int*)arg1);
        }
        

        /* void* tmp = (void*) calloc(1, GET_ARG(cmd));
        memcpy(tmp, program + adress, sizeof(char));

        printf("%d\n", *(int*)tmp ); */
        
        // fprintf(lst_file, "|%12d|     --     |\n", GET_ARG(cmd));
    }
    else if (arg_num == 2) {


        int arg_size = GET_ARG(cmd);

        if (arg_size == 0) {
            arg1 = (void*) calloc(1, arg_size);
            memcpy(arg1, program + adress, arg_size);
            // fprintf(lst_file, "| label (0x%2hhx) |      --      |\n", *(int*)arg1);
        }
        else {
            arg1 = (void*) calloc(1, arg_size);
            memcpy(arg1, program + adress, arg_size);
            // if (arg_size == sizeof(float)) fprintf(lst_file, "|%14f|      --      |\n", *(float*)arg1);
            // else                           fprintf(lst_file, "|    reg%3d    |      --      |\n", *(int*)arg1);
        }
        
        // int arg_1 = *(int*)(adress + sizeof(char));
        // fprintf(lst_file, "|%14d|%14d|\n", arg_1, arg_2);
    }
}

