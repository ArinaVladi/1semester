#include "D:\MainFolder\repos\computer3\cpu\processor\cpu.h"

int main(int argc, char* argv[]) {

    cpu Cpu = {};

    #if defined LISTING
    if (argc < 3) {
        printf("build-debug/cpu.exe <binary file> <listing file>");
        assert(0 && "file names were not prowided!\n");
    }
    #else
    if (argc < 2) {
        printf("build-debug/cpu.exe <binary file> <listing file>");
        assert(0 && "file names were not prowided!\n");
    }
    #endif

    else {
        Cpu.file_name   = argv[1];
        IF_LISTING(Cpu.listig_file_name   = argv[2]);
    }
    

    
    Cpu.main();  

    return 0;
}


