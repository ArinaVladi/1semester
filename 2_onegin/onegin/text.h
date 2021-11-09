#ifndef ONEGIN_ONE_FILE_TEXT_H
#define ONEGIN_ONE_FILE_TEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <string.h>
#include <ctype.h>

#include "my_utilities.h"
#include "macroUtilities.h"
#include "files.h"

enum {
    ALPHABET = 1, RHYME = 2, INITIAL = 3
};

struct Line {
    char *index;
    int length;
};

struct Text {  // typedef


    char          *wholeText;
    int               nLines;
    unsigned long   nSymbols;
    Line              *lines;

    void Ctor(struct Files *theFiles);
    void sort_( int    (*cmp_wrapper_alphabetic)(const void *, const void *),
                int    (*cmp_wrapper_rhyme)(const void *, const void *), FILE *out_f);
    void write_buf(FILE *file, int nSymbols);
    void Dtor(char *wholeText, Line* lines);


};

void get_ptr_lines(struct Text *);
void write_lines(struct Line *, int, FILE *);
void sort_certain_way(Line *, int, int (*)(const void *, const void *), FILE *, int);
int  cmp_wrapper_alphabetic (const void *v1, const void *v2);
int  cmp_wrapper_rhyme (const void *v1, const void *v2);

#endif //ONEGIN_ONE_FILE_TEXT_H
