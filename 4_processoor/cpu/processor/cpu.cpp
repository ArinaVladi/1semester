#include "cpu.h"

#include <SFML/Graphics.hpp>

const int COMMAND_TYPE_MASK = 31;

// char* cpu::file_name = "out.txt";
// char* cpu::listig_file_name = "listing.txt";

void cpu::main() {

    Ctor();

    read_program_file();

    execute_command();
} 

void cpu::Ctor() {

    theStack = {};
    IP = 0;
    int st = theStack.Ctor(sizeof(float));

    IF_FLOAT(ram = (float*) calloc(RAM_SIZE, sizeof(float)));
    IF_FIXED(ram = (int*)     calloc(RAM_SIZE, sizeof(int)));
    assert(ram != nullptr);

    listing = {};
    IF_LISTING(listing.open_and_init_file(listig_file_name));   
}

unsigned long get_file_size_fstat (char* f_name) {

    assert (f_name != nullptr);

    struct stat file_stat;
    assert (stat (f_name, &file_stat) != -1);
    return file_stat.st_size;
}

void cpu::read_program_file() {

    file_size = get_file_size_fstat(file_name);

    program = (char*) calloc(file_size+1, sizeof(char)); // +1 потому что указатель сразу увеличивается после считывания команды
    assert(program != nullptr);

    file = fopen (file_name, "rb");
    fread(program, sizeof(char), file_size, file);

    check_bin_file();
}

void cpu::check_bin_file() {

    void* tmp_header = (void*)calloc(3, sizeof(char));
    memcpy(tmp_header, (void*)(program + IP), 3*sizeof(char));
    assert ((*(int*)tmp_header == HEADER) && "Header is wrong!\n");
    IP += 3;

    void* tmp_version = (void*)calloc(1, sizeof(char));
    memcpy(tmp_version, (void*)(program + IP), sizeof(char));
    assert((*(int*)tmp_version == VERSION) && "Version is wrong!\n");
    IP += 1;

    IF_LISTING(listing.print_file_header((char*)tmp_header, *(int*)tmp_version));

    free(tmp_version);
    free(tmp_header);
} 

void cpu::execute_command(){

    int n = 0;

    while (IP < file_size) {

        int cur_cmd = CUR_COMMAND;
        IF_LISTING(int ip_prev = IP);

        #define COMMAND(name, num, arg_num, param_type, code) case COMMAND_ ## name :                                  \
                code                                                                                             \
                IF_LISTING(listing.document_command(ip_prev, #name, num, arg_num, param_type, program, cur_cmd);) }                                \                                                               
                break; 

        switch(cur_cmd & COMMAND_NAME_MASK){

            #include "D:\MainFolder\repos\computer3\cpu\cmd_def.h"

            default:
                printf("IP = %d\n", IP);
                assert(0 && "Wrong command name");
        }

        #undef COMMAND
        n++;
    }
}

//--------------------------------------read necessary ammpunt of bytes from bin file-----------------------------------

#if defined FLOAT

float* cpu::get_cur_arg(int size) {

    void* cur_arg = (void*)calloc(1, sizeof(float));
    memcpy(cur_arg, (void*)(program + IP), size);
    IP += size;

    return (float*)cur_arg;
}

float* cpu::get_cur_reg() {

    float* ret_reg = GET_REG_NUM;
    IP+=1;
    return ret_reg;
}

#else

int* cpu::get_cur_arg(int size) {

    void* cur_arg = (void*)calloc(1, sizeof(float));
    memcpy(cur_arg, (void*)(program + IP), size);
    IP += size;

    return (int*)cur_arg;
}

int* cpu::get_cur_reg() {

    int* ret_reg = GET_REG_NUM;
    IP+=1;
    return ret_reg;
}

#endif

//---------------------------------------------------------Dtor---------------------------------------------------------

void cpu::Dtor() {
    free(ram);
}

//--------------------------------------------------functions for dump--------------------------------------------------

#define LOCATION "%s %s %d\n", __FILE__, __func__, __LINE__
#define DUMP \
    printf(LOCATION); \
    dump();

void cpu::dump() {

    printf("Index Pointer = %d\n", IP);
    printf("| 00| 01| 02| 03| 04| 05| 06| 07| 08| 09| 10| 11| 12| 13| 14|\n");

    for (int i = 0, char_count = 0, ip_row = (IP*4)/60, ip_column = (IP)%15; i < file_size; i++) {
        printf("|%3d", program[i]);
        char_count+=4;
        if(char_count >= 60) {
            char_count = 0;
            ip_row--;
            if (ip_row < 0){
                printf("\n");
                while (ip_column-- > 0) {
                    printf("    ");
                }
                printf("  ^\n");
                break;
            }
            printf("|\n");
        }
    }
    printf("\n\n");
}