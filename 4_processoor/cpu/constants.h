#ifndef CPU_CONSTANTS_H
#define CPU_CONSTANTS_H

//------------------------------------------------------metadata--------------------------------------------------------

const int METADATA_SIZE = 4;
const int VERSION = 4;
const int HEADER = 'Hh8';

#include "D:\MainFolder\repos\computer3\cpu\another_constants.h"
#define COMMAND(name, num, arg_num, param_type, code) COMMAND_ ## name = num,
enum COMMANDS {

    #include "D:\MainFolder\repos\computer3\cpu\cmd_def.h"

};
#undef COMMAND

#endif //CPU_CONSTANTS_H
