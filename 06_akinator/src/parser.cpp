#include "parser.h"

void Parser::Ctor(char* infile_name) {
    file_name = infile_name;
    file = fopen(file_name, "r");
}

void Parser::Dtor() {

    fclose(file);
    free(syntagms);    
    free(buf);
}

void Parser::pars() {
    
    read_file();
    syntagms = (syntagm_t*)calloc(count_syntagms(), sizeof(syntagm_t));
    get_syntagms_ptr();
}

//---------------------------------------------------------------------work with file----------------------------------------------------------------------

unsigned long Parser::get_file_size_fstat (char* f_name) {

    assert (f_name != nullptr);

    struct stat file_stat;
    assert (stat (f_name, &file_stat) != -1);
    return file_stat.st_size;
}

void Parser::read_file() {

    file_size = get_file_size_fstat(file_name);
    assert(file_size > 0);

    buf = (char *) calloc(file_size+1, sizeof(char));
    assert(buf != nullptr);
    assert(fread(buf, sizeof(char), file_size+1, file));
}

//--------------------------------------------------------------------------parsing--------------------------------------------------------------------------

int Parser::count_syntagms() {
    
    char *buf_tmp = buf;
    nLines = 0;

    int tmp_count = 0;

    for (int i = 0; i < file_size; i++) {       
        while (isspace(buf[i]) || buf[i] == '\0') {
            i++;
        }
        if (buf[i] == '<') {
            nLines++;
            while (buf[i++] != '>');
            buf[i-1] = '\0';
        }
        else if (buf[i] == '{' || buf[i] == '}') {
            nLines++;
            i++;
            buf[i] = '\0';
        }
    }  

    return nLines;
}

void Parser::get_syntagms_ptr() {

    char* buf_tmp = buf;
    syntagm_t* syntagms_tmp = syntagms;

    for (int i = 0; i < file_size; i++) {

        while (isspace(buf[i]) || (buf[i] == '\0')){
            i++;
        }

        if ((buf[i] == '{') || (buf[i] == '}')) {
            syntagms_tmp->index = buf + i;
            i++;
        }
        else if (buf[i] == '<') {
            i++;
            syntagms_tmp->index = buf + i;
            while(buf[i++] != '\0');
        }
        syntagms_tmp++;
    } 
}

//------------------------------------------------------------------------auxilary-------------------------------------------------------------------------

int get_line(char *buffer, int bsize)
{
    int ch, len;

    fgets(buffer, 1000, stdin);
    
    // remove unwanted characters from the buffer
    buffer[strcspn(buffer, "\r\n")] = '\0';

    len = strlen(buffer);

    // clean input buffer if needed 
    if(len == bsize - 1)
        while ((ch = getchar()) != '\n' && ch != EOF);

    return len;
}