#include  <stdio.h>
#include <stdlib.h>
#include  <cassert>
#include <typeinfo>
#include  <cstring>
#include <sys/stat.h>
#include <ctype.h>

typedef struct Syntagm {
    
    char *index;
    int  length;
} syntagm_t;

typedef struct {

    char                  *buf;
    int                 nLines;
    syntagm_t        *syntagms;
    unsigned long    file_size;
    char*            file_name;
    FILE*                 file;

    char*                 code;
    int                     ip;

    unsigned long get_file_size_fstat (char* f_name);

    void     pars();
    void     read_file();
    int      count_syntagms();
    void     get_syntagms_ptr();
  
    void     Dtor();
    void     Ctor(char* file_name);    
} Parser;

int get_line(char *buffer, int bsize);
