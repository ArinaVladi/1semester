#ifndef CPU_ASSEMBLER_H
#define CPU_ASSEMBLER_H

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
#define IF_FLOAT(code) code
#else
#define IF_FLOAT(code)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <cstdint>
#include <cstring>

#include "D:\MainFolder\repos\computer3\cpu\utilities.h"
#include "D:\MainFolder\repos\computer3\cpu\constants.h"

const int MAX_INT_NUM =   6;
const int NUM_LABLES  = 100;
const int NUM_JUMPS   = 100;

typedef struct Command_info {

    int          cmd_hash;
    unsigned int  arg_num;
    unsigned int     code;
} command_info;

typedef struct Command_to_cpu { // 00000 0 0 0

    unsigned int code     : 5;
    unsigned int is_const : 1;
    unsigned int is_reg   : 1;
    unsigned int is_ram   : 1;
} command_to_cpu_t;

typedef struct Syntagm {
    char *index;
    int  length;
} syntagm_t;

typedef struct Regs {
    int    num;
    char* name;
} reg_t;

typedef struct Labels {
             char* name;
    IF_FLOAT(float  ip);
    IF_FIXED(int    ip);
} label_t;

typedef struct Jmp {
             char*  label_name;
    IF_FLOAT(float jmp_arg_ip);
    IF_FLOAT(float   label_ip);
    IF_FIXED(int   jmp_arg_ip);
    IF_FIXED(int     label_ip);
} jmp_t;

typedef struct Assembler {

    char                  *buf;
    int                 nLines;
    syntagm_t        *syntagms;
    unsigned long    file_size;
    char*          output_name;
    char*            file_name;
    FILE*               output;
    FILE*                 file;

    int             labels_num;
    label_t*            labels;

    int                jmp_num;
    jmp_t*                jmps;

    char*                 code;
    int                     ip;

    void     main();
    void     open_files();
    void     clear_file ();

    void     pars();
    void     read_file();
    int      count_syntagms();
    void     get_syntagms_ptr();
    void     compare_and_set_equivalence();
    
    void     process_label(char* word);
    void     process_arguments(PARAM_TYPES param_type, int* syntagm_num, command_to_cpu_t* cur_command, int arg_num);
    IF_FLOAT(int      get_arg_type(int* syntagm_num, float* argument, char* reg_name));
    IF_FIXED(int      get_arg_type(int* syntagm_num, int*   argument, char* reg_name));
    void     push_pop_arg_processing(int* syntagm_num, command_to_cpu_t* cur_command, int arg_num,  int param_type);
    void     jump_processing(int* syntagm_num, command_to_cpu_t* cur_command, FILE* output, int arg_num);
    void     complete_jmp_addresses();

    void     write_meta_data();
    void     write_to_bin_file();
    
    void     Dtor();
    void     Ctor(int num_lables_to_alloc, int num_jumps_to_alloc);
    
} assembler;

#endif //CPU_ASSEMBLER_H
