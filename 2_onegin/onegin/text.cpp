#include "text.h"

void count_and_initialize_lines(struct Text *theText) {

    assert (theText != NULL);

    char *wholeText = theText->wholeText;
    int nSymbols = theText->nSymbols;

    PD_Value ("%d %s %c\n\n", __LINE__, __func__, *(theText->wholeText));

    char *ptr_newLine = wholeText;
    while (ptr_newLine = strchr(ptr_newLine, '\n')) {
        *ptr_newLine = '\0';
        ptr_newLine++;
        theText->nLines += 1;
    }
    Print_Debug ("Number of lines is calculated,  \\n were changed to \\0");
}


void get_ptr_lines(struct Text *theText) {

    assert (theText != NULL);
    assert (theText->nLines > 0);

    theText->nLines =                  0;
    int nSymbols    =  theText->nSymbols;
    char *wholeText = theText->wholeText;
    Line *lines     =     theText->lines;
    lines->index    =          wholeText;

    Print_Debug ("Before array of pointers initialisation");

    while (nSymbols > 0) {
        while (compare(*wholeText, IsSpace|IsNotLineBegin)) {
            wholeText++;
            nSymbols--;
        }

        if (*(wholeText++) == '\0') {
            lines->length = wholeText - lines->index - 1;

            while (compare((unsigned char) *wholeText, IsSpace|IsNotLineBegin)) {
                PD_Value("\tSymbol to delete %d %c. IsSpace %d \n", (unsigned char) *wholeText, *wholeText, compare(*wholeText, IsSpace));
                wholeText++;
                nSymbols--;
            }

            (lines + 1)->index = wholeText;
            lines++;
            theText->nLines += 1;
            nSymbols--;
        }
        nSymbols--;
    }

    PD_Value ("##nLines in ptr_lines = %d\n", theText->nLines);
    PD_Value ("Last symbol in ptr_lines-1 = %c\n\n", *(theText->lines->index) /* *(*ptr_lines-1)*/);
    Print_Debug ("Get pointers to the beginnings od the lines");
}

//-----------------------------------------------------------------------------

void write_lines(Line *lines, int nLines, FILE *out_f) {
    assert (lines != NULL);
    assert (nLines > 0);
    assert (out_f != NULL);


    for (int i = 0; i < 5; i++) {
        fprintf (out_f, "--------------------------------------------------------------------------------\n");
    }
    while (nLines-- > 0) {

        if (*lines->index != '\0' && *(lines + 1)->index != '\0') {        // ?
            fprintf(out_f, "|%s|\n", lines->index);
            lines++;
        }
    }

    Print_Debug ("Lines ara printed in file");
}

void sort_certain_way(Line *lines, int nLines, int (*cmp)(const void *, const void *), FILE *out_f, int key) {

    my_qsort(lines, nLines + 1, sizeof(Line), cmp);
    write_lines(lines, nLines + 1, out_f);
}

void Text::Ctor(struct Files *theFiles) {

    Print_Debug("Text constructor starting");

    this->wholeText = (char *) calloc(theFiles->fileSize + sizeof(char), sizeof(char));
    assert (this->wholeText != NULL);

    Print_Debug("memory for text is allocated");

//    this->nSymbols = read(theFiles->in_fd, this->wholeText, theFiles->fileSize);
//    this->nSymbols = fread( this->wholeText, sizeof(char), theFiles->fileSize, theFiles->in_f);
    //char* buf = NULL;

   // rewind(theFiles->in_f);

    PD_Value("#####first character is %c#####\n", fgetc(theFiles->out_f));

    PD_Value("fread returns %zu\n", fread(this->wholeText, sizeof(char), theFiles->fileSize/sizeof(char)-1, theFiles->in_f));
    PD_Value("FILE* is %d\n", theFiles->fileSize/sizeof(char));
    assert (wholeText);
    PD_Value("length = %d\n", strlen(wholeText));
    assert (strlen(wholeText)>0);
    Print_Debug("wholeText is read");

    char* wholeText_cpy = wholeText;
    while (wholeText_cpy++) {
        nSymbols++;
    }
    *(this->wholeText+this->nSymbols) = '\0';

    PD_Value ("!!!!! Symbol А = %d\n\n", this->wholeText);
    assert (this->nSymbols > 0);
    assert (this->wholeText[0] != 0);
    PD_Value ("nSymbols = %lu\n\n", this->nSymbols);

    this->nLines = 0;

    count_and_initialize_lines(this);
    PD_Value ("%d %s: nLines = %d\n", __LINE__, __func__, this->nLines);     // ?
    PD_Value  ("nSymbols = %d\n", this->nSymbols);
    PD_Value ("Symbol 0 = %c\n\n", *this->wholeText); //A

    this->lines = (Line *) calloc(this->nLines, sizeof(Line));
    assert (this->lines != NULL);
    PD_Value ("##nLines before = %d\n", this->nLines);
    get_ptr_lines(this);
}

void Text::write_buf(FILE *file, int nSymbols) {

    char *buf = this->wholeText;
    assert (buf != NULL);
    assert (file != NULL);
    assert (nSymbols > 0);

    while (--nSymbols > 0) {

        if (*buf != '\0') {
            fputc(*buf++, file);
        } else {
            fputc('\n', file);
            buf++;
        }
    }
    Print_Debug ("Text is printed in file");
}

int cmp_wrapper_alphabetic (const void *v1, const void *v2) {
    my_cmp_2(v1, v2, 1);
}

int cmp_wrapper_rhyme (const void *v1, const void *v2) {
    my_cmp_2(v1, v2, -1);
}

void Text::sort_(int (*cmp_wrapper_alphabetic)(const void *, const void *),
                 int (*cmp_wrapper_rhyme)(const void *, const void *), FILE *out_f) {

    sort_certain_way(this->lines, this->nLines, cmp_wrapper_alphabetic, out_f,ALPHABET);
    sort_certain_way(this->lines, this->nLines, cmp_wrapper_rhyme, out_f,RHYME);
}


void Text::Dtor(char *wholeText, Line* lines) {
    free(wholeText);
    free(lines);
}
