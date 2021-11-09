#ifndef CPU_CPU_H
#define CPU_CPU_H

#include "stack.h"
#include "D:\MainFolder\repos\computer3\cpu\utilities.h"
#include "fixed_point.h"
#include "D:\MainFolder\repos\computer3\cpu\constants.h"
#include "listing.h"
#include "stack.h"

#define LISTING
// #define FIXED
#define FLOAT

#if defined LISTING
#define IF_LISTING(code) code
#else
#define IF_LISTING(code)
#endif

#if defined FIXED
#define IF_FIXED(code) code
#else
#define IF_FIXED(code)
#endif

#if defined FLOAT
#define IF_FLOAT(code) code;
#else
#define IF_FLOAT(code)
#endif

const float PUSH_VAL        = 13.24f;
const int   RAM_SIZE        =   3000;
const int  VM_OFFSET        =     50;
const int COMMAND_NAME_MASK =     31;

typedef struct Cpu {

    char*          file_name;
    FILE*               file;
    char*            program;
    int            file_size;
    char*   listig_file_name;
    listing_t        listing;

    stack_t         theStack;


    #if defined FLOAT
        float*           ram;
        float          reg_1; // ax
        float          reg_2; // bx
        float          reg_3; // cx
        float          reg_4; // dx
        float          reg_5; // tx

    #else
        int*             ram;
        int            reg_1; // ax
        int            reg_2; // bx
        int            reg_3; // cx
        int            reg_4; // dx
        int            reg_5; // tx
    #endif

    
    int                   IP;
    int                   CF; // carry
    int                   SF; // sign
    int                   ZF; // zero

    /*Assume result = op1 - op2

    CF - 1 if              unsigned op2 > unsigned op1
    OF - 1 if    sign bit of OP1 != sign bit of result
    SF - 1 if         MSB (aka sign bit) of result = 1
    ZF - 1 if                Result = 0 (i.e. op1=op2)
    AF - 1 if        Carry in the low nibble of result
    PF - 1 if Parity of Least significant byte is even */

             void                   main();
             void      read_program_file();
             void        execute_command();
             void                   dump();
             void         check_bin_file();
    IF_FLOAT(float* get_cur_arg(int size));
    IF_FIXED(int*   get_cur_arg(int size));
    IF_FLOAT(float*         get_cur_reg());
    IF_FIXED(int*           get_cur_reg());

            void                    Ctor();
            void                    Dtor();

} cpu;

#endif //CPU_CPU_H
