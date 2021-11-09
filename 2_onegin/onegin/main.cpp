#include <stdio.h>
#include <cassert>

#include "setRus.h"
#include "text.h"
#include "files.h"

int main (int argc, char *argv[]) {

    assert (argc > 0);
    set_rus ();
    Print_Debug ("Start");

    // инициализация структур
    struct Files theFiles = {};
    theFiles.Ctor(argc, argv);

    struct Text theText = {};
    theText.Ctor( &theFiles);

    // вызываются две сортировки
    theText.sort_(cmp_wrapper_alphabetic, cmp_wrapper_rhyme, theFiles.out_f);

    // запись изначально в файл
    theText.write_buf(theFiles.out_f, theText.nSymbols + 1);

    // деструкторы
    theFiles.Dtor();
    theText.Dtor(theText.wholeText, theText. lines);

    return 0;
}