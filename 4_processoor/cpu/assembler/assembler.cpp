#include <sys/stat.h>
#include <cctype>

#include "assembler.h"

//---------------------------------------------constant command specifiers----------------------------------------------

const int CMD_NUM = 27;
const int ARG_REG_X_POSITION = 2;
const int ARG_RAM_REG_FIRST_X_POSITION = 3;
const int ARF_RAM_REG_SECOND_X_POSITION  = 6;
const int TEMPORARILY_EMPTY = -7;

reg_t REGS[] = {
        {0, "err"},
        {1, "a"},
        {2, "b"},
        {3, "c"},
        {4, "d"},
        {5, "t"}
};

//-----------------------------------------------main assembler function------------------------------------------------

int main(int argc, char* argv[]) {

    assembler assm = {};
    assm.Ctor(NUM_LABLES, NUM_JUMPS);

    if (argc < 3) {
        printf("build-debug/assm.exe <assembler programm> <output file>");
        assert(0 && "file names were not prowided!\n");
    }

    else {
        assm.file_name   = argv[1];
        assm.output_name = argv[2];
    }

    assm.open_files();
    assm.pars();

    assm.write_meta_data();
    fwrite(assm.code, sizeof(char), assm.ip, assm.output);
    assm.Dtor();  

    return 0;
}


void assembler::main() {
    
    Ctor(NUM_LABLES, NUM_JUMPS);
    open_files();
    pars();

    write_meta_data();
    fwrite(code, sizeof(char), ip, output);
    Dtor();
}


void assembler::Ctor(int num_lables_to_alloc, int num_jumps_to_alloc) {

    ip         = 4;
    labels_num = 0;
    labels     = (label_t*)calloc(num_lables_to_alloc, sizeof(label_t));
    jmp_num    = 0;
    jmps       = (jmp_t*)calloc(num_jumps_to_alloc, sizeof(jmp_t));
}

void assembler::open_files() {

    clear_file();

    file   =    fopen(file_name, "r");
    output = fopen(output_name, "ab");

    assert   (file != nullptr);
    assert (output != nullptr);
}

void assembler::clear_file () {

    output = fopen (output_name, "w");
    assert (file_name);
    fclose    (output);
}

void assembler::pars() {
    
    read_file();

    syntagms = (syntagm_t*)calloc(count_syntagms(), sizeof(syntagm_t));
    get_syntagms_ptr();

    code = (char*)calloc(nLines + METADATA_SIZE + 1, sizeof(int)); 

    compare_and_set_equivalence();

    if (jmp_num > 0) complete_jmp_addresses();
}

unsigned long get_file_size_fstat (char* f_name) {

    assert (f_name != nullptr);

    struct stat file_stat;
    assert (stat (f_name, &file_stat) != -1);
    return file_stat.st_size;
}

void assembler::read_file() {

    file_size = get_file_size_fstat(file_name);
    assert(file_size > 0);

    buf = (char *) calloc(file_size+1, sizeof(char));
    assert(buf != nullptr);
    assert(fread(buf, sizeof(char), file_size+1, file));
}

//-------------------------------------------------------parsing--------------------------------------------------------

int assembler::count_syntagms() {
    
    char *buf_tmp = buf;
    nLines = 0;

    int counter = file_size;

    int tmp_count = 0;

    for (int i = 0; i < file_size; i++) {
        while (isspace(buf_tmp[i])){
            i++;
        }
        while (isalpha(buf[i]) || isdigit(buf[i]) || ispunct(buf[i])) {
            i++;
        }
        buf[i] = '\0';
        nLines++;
    }
    return nLines;
}

void assembler::get_syntagms_ptr() {

    char* buf_tmp = buf;
    syntagm_t* syntagms_tmp = syntagms;

    for (int i = 0; i < file_size; i++) {

        while (isspace(buf_tmp[i]) || (buf_tmp[i] == '\0')){
            i++;
        }

        syntagms_tmp->index = buf_tmp+i;

        while (buf_tmp[i] != '\0' ) {
            i++;
        };

        syntagms_tmp++;
    }
}

//------------------------process syntagms and write command codes and arguments to binary file-------------------------

void set_command_code(int code_to_set, command_to_cpu_t* cur_command) {
    cur_command->code = code_to_set;
}

void assembler::compare_and_set_equivalence() {

    for (int i = 0; i < nLines; i++) {

        command_to_cpu_t cur_command = {};
        char* word = (syntagms+i)->index;

        if (word[strlen(word)-1] == ':') {

            process_label(word);
            continue;
        }

        else {

            #define COMMAND(name, num, arg_num, param_type, code)                              \
                if(strcmp(#name, word) == 0) {                                                 \
                    set_command_code(num, &cur_command);                                       \
                    process_arguments(param_type, &i, &cur_command, arg_num);                  \
                } 

            #include "D:\MainFolder\repos\computer3\cpu\cmd_def.h"
            #undef COMMAND         
        }
    }
}

void assembler::process_arguments (PARAM_TYPES param_type, int* syntagm_num, command_to_cpu_t* cur_command, int arg_num) {

    switch (param_type) {                                               
        case ALL: case _RET_:                                               
            push_pop_arg_processing(syntagm_num, cur_command, arg_num, param_type);       
            break;                                                   
                                                           
        case LABEL:                                     
            jump_processing (syntagm_num, cur_command, output, arg_num);     
            break;         

        case NO:                                                 
            memcpy (code + ip, cur_command, sizeof(char));             
            ip += 1;                                                 
            break;    

        default:                                                     
            assert (0 && "ERROR in some operation's param_type in cmd_def.h file");  
    }                                                                               
}

void assembler::push_pop_arg_processing(int* syntagm_num, command_to_cpu_t* cur_command, int arg_num, int param_type) {

    IF_FLOAT(float argument =  0);
    IF_FIXED(int   argument =  0);
             char  reg_name = {};
             int   arg_type =  0;
             int   reg_num  =  0;

    for (int i = 0; i < arg_num; i++) {

        (*syntagm_num)++;

        cur_command->is_const = 0;
        cur_command->is_reg   = 0;
        cur_command->is_ram   = 0;

        arg_type = get_arg_type(syntagm_num, &argument, &reg_name);

        if (ERROR == (arg_type & ERROR)) {
            if (param_type == _RET_) {

                argument = 0;
                arg_type = CONST_;
            }
            else {
                assert ("Syntax ERROR\n" && 0);
            }
        }

        if (CONST_ == (arg_type & CONST_)) {
            cur_command->is_const = 1;
        }

        if (REG_ == (arg_type & REG_)) {

            cur_command->is_reg = 1;

            for (int i = 0; i < MAX_INT_NUM; i++) {

                if ((uint64_t)reg_name == (int)REGS[i].name[0]) {
                    reg_num = REGS[i].num;
                    break;
                }
            }
        }

        if (RAM == (arg_type & RAM)) {
            cur_command->is_ram = 1;
        }

        memcpy(code+ip, cur_command, sizeof(char));
        ip += sizeof(char);
        if (REG_ == (arg_type & REG_)) {
            memcpy(code+ip, &reg_num, sizeof(char));  //? два раза повторяется один и тот же фрагмент для REG и CONST
            ip += 1;
        }

        if (CONST_ == (arg_type & CONST_) || (param_type == _RET_)) {
            memcpy(code+ip, &argument, sizeof(argument));
            ip += sizeof(argument);
        }
    }  
}

void assembler::jump_processing (int* syntagm_num, command_to_cpu_t* cur_command, FILE* output, int arg_num) {

    // cur_command - текущий jmp

    (*syntagm_num)++;
    jmps[jmp_num].label_name = (syntagms + *syntagm_num)->index; //  label_name
    jmps[jmp_num].jmp_arg_ip = ip + sizeof(char);
    jmps[jmp_num].label_ip   = TEMPORARILY_EMPTY;
    jmp_num++;

    memcpy(code+ip, cur_command, sizeof(char));
    ip +=  sizeof(char);

    memcpy(code+ip, &TEMPORARILY_EMPTY, sizeof(uint32_t));
    ip +=  sizeof(uint32_t);
}

void assembler::process_label(char* word) {

    word[strlen(word)-1]    = '\0';
    labels[labels_num].name = word;
    labels[labels_num].ip   =   ip;
    labels_num++;
}

void assembler::complete_jmp_addresses() {

    for (int jmp_count = 0; jmp_count < jmp_num; jmp_count++) {

        for (int label_count = 0; label_count < labels_num; label_count++) {

            if (strcmp(labels[label_count].name, jmps[jmp_count].label_name) == 0) {

                if (jmps[jmp_count].label_ip != TEMPORARILY_EMPTY) {
                    assert(0 && "Label has already been declared");
                }

                else {
                    IF_FLOAT(float label_ip = labels[label_count].ip);
                    IF_FIXED(int   label_ip = labels[label_count].ip);
                    memcpy(code + (int)jmps[jmp_count].jmp_arg_ip,  &(labels[label_count].ip), sizeof(int));
                    jmps[jmp_count].label_ip = labels[label_count].ip;
                }

                break;
            }
        }
    }
}

//-------------------------------------------------arguments processing-------------------------------------------------

IF_FLOAT(int assembler::get_arg_type(int* syntagm_num, float* argument, char* reg_name))
IF_FIXED(int assembler::get_arg_type(int* syntagm_num, int*   argument, char* reg_name)) {

    const char* str         = (syntagms + *syntagm_num)->index;
    int         position    = 0; 
    int         arg_type    = NOT_INIT;            
            
    arg_type = ((sscanf(str, "[%1[abcdt]x%n+%f]", reg_name, &position, argument)==2)                           
                && (position == ARG_RAM_REG_FIRST_X_POSITION) && (*(int*)argument != 0))?      (RAM | REG_ | CONST_) :
                    (((sscanf(str, "[%f+%1[abcdt]x%n]", argument, reg_name, &position)==2)   
                    && (*(int*)argument != 0))? (RAM | REG_ | CONST_) : 
                        ((sscanf(str, "%f", argument) == 1)?                                   CONST_                :
                            (((sscanf(str, "%1[abcdt]x%n", reg_name, &position) == 1)        
                            && (position == ARG_REG_X_POSITION))?                              REG_                  : 
                                (((sscanf(str,"[%1[abcdt]x%n]", reg_name, &position)==1)
                                && (position == ARG_RAM_REG_FIRST_X_POSITION))?                (RAM | REG_ )         :
                                    ((sscanf(str, "[%f]", argument)==1)?                       (RAM | CONST_)        : 
                                                                                               ERROR))))) ;      
    
    return (arg_type);
}

//------------------------------------------------writing to binary file------------------------------------------------

void assembler::write_meta_data() {

    memcpy(code, &HEADER, 3 * sizeof(char));
    memcpy(code + 3 * sizeof(char), &VERSION, sizeof(char));
}

void assembler::Dtor() {

    fclose(output);
    fclose(file);
    free(code);
    free(syntagms);    
    free(buf);
    free(labels);
}