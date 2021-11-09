//
// Created by ArinaVladi on 20.09.2021.
//

#ifndef ONEGIN_ONE_FILE_FILES_H
#define ONEGIN_ONE_FILE_FILES_H

#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

#include "macroUtilities.h"
#include "symbols_table.h"

struct Files {

    FILE *in_f;
    FILE *out_f;
    int in_fd;
    int out_fd;
    char *f_in_name;
    char *f_out_name;
    unsigned long fileSize;

    void Ctor (int, char **);
    void Dtor ();
};

void process_cmd_arg(char **, char **, int, char **);
void open_files(FILE *, FILE *, char *, char *);
unsigned long get_file_size_fstat(char *);

#endif //ONEGIN_ONE_FILE_FILES_H
