#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <cassert>
#include <cstring>

#include "setRus.cpp"

enum {  IsAlpha        =  1,
        IsDigit        =  2,
        IsRomanDigit   =  4,
        IsNotLineBegin =  8,
        IsPunct        = 16,
        IsStrangeDigit = 32,
        IsSpace        = 64,
        IsUpper        = 128,
        IsLower        = 256
};

const char*  mask_names[9] = {"IsAlpha",        // 0
                             "IsDigit",        // 1
                             "IsRomanDigit",   // 2
                             "IsNotLineBegin", // 3
                             "IsPunct",        // 4
                             "IsStrangeDigit", // 5
                             "IsSpace",        // 6
                             "IsUpper",        // 7
                             "IsLower"         // 8
                          };


#define set_mask(start, end, mask) for (int i = start; i <= end; i++) Symbols[i] = Symbols[i] | mask;

int Symbols[256] = {};

void set_masks ();
void my_strcpy (char* dest, const char* s );

int main() {

    set_rus();

    FILE* symb_array = fopen("array_of_symb.txt", "w");
    assert (symb_array);
    int symb_num = 0;
    char *mask_name;
    set_masks();

    fprintf(symb_array, "int Symbols[256] = {\n");

    for (symb_num = 0; symb_num < 256; symb_num++) {
        printf ("\nChecking symbol %d\n", symb_num);
        printf ("Symbol mask is %d\n", Symbols[symb_num]);
        //char symb[20];
        char symb = (char)symb_num;
        char buf[300]={};
        int len = 0;
       // symb_masks = (char*)1;
        for (int mask = 1, mask_num = 0; mask <= 256; mask *= 2, mask_num ++){
            printf ("-Checking mask %d %d\n", mask_num, mask);

            if ((Symbols[symb_num] & mask) == mask) {
                printf("\tSymbol \"%c\" has the mask %d %s\n", symb, mask, mask_names[mask_num]);

                //my_strcat(symb_masks,  mask_names[mask_num]);
                if (len > 0) {
                    len += 1;
                   // *(buf+len) = '_';
                    buf[len-1] = ' ';
                    //*(buf+len+1) = '\0';

                    printf("\t\tNow mask is %s and last symbol is %c\n", buf, *(buf+len));
                   // *(buf+len+2) = '\0';
                    strcpy(buf+(len),  mask_names[mask_num]);
                }
                else {
                    strcpy(buf+len,  mask_names[mask_num]);
                }
                printf("\tNow symbol mask is %s\n", buf);
                len += strlen(mask_names[mask_num]);
            }
        }

        if (symb == '\t' || symb == '\n' || symb == '\r') { //|| (char)symb_num == '\n' || (char)symb_num == '\r')
           symb = '_';
       }

       char* ptr_space = buf;

       while (ptr_space = strchr(buf, ' ')) {
           *ptr_space = '|';
           ptr_space++;
       }
       /*if (symb_num >= 28 && symb_num <= 31) {
            fprintf(symb_array, "\t%-47s, // %3d  \"%c\"\n", buf, symb_num, ' ');
        }*/
      /* else*/ if (symb_num == '\t') {
           fprintf(symb_array, "\t%-47s, // %3d \"\\t\"\n", buf, symb_num);
       }
       else if (symb_num == '\n') {
           fprintf(symb_array, "\t%-47s, // %3d \"\\n\"\n", buf, symb_num);
       }
       else if (symb_num == '\v') {
           fprintf(symb_array, "\t%-47s, // %3d \"\\v\"\n", buf, symb_num);
       }
       else if (symb_num == '\r') {
           fprintf(symb_array, "\t%-47s, // %3d \"\\r\"\n", buf, symb_num);
       }
       else if (symb_num <= 254) {
           fprintf(symb_array, "\t%-47s, // %3d  \"%c\"\n", buf, symb_num, symb);
       }
       else {
           fprintf(symb_array, "\t%-47s  // %3d  \"%c\"\n", buf, symb_num, symb);
       }
    }
    fprintf(symb_array, "};\n");
    return 0;
}

void set_masks () {

    set_mask(0, 31, IsStrangeDigit);

    set_mask(9, 11, IsSpace);
    set_mask(9, 11, IsNotLineBegin);
    set_mask(9, 11, IsPunct);

    set_mask(13, 13, IsSpace);
    set_mask(13, 13, IsNotLineBegin);
    set_mask(13, 13, IsPunct);

    set_mask(32, 32, IsSpace);
    set_mask(32, 32, IsNotLineBegin);
    set_mask(32, 32, IsPunct);

    set_mask(32, 47, IsPunct);

    set_mask(42, 42, IsNotLineBegin); // *

    set_mask(46, 46, IsNotLineBegin); // .

    set_mask(48, 57, IsDigit);
    set_mask(58, 64, IsPunct);

    set_mask('A', 'Z', IsAlpha);
    set_mask(65, 90, IsUpper);

    set_mask(67, 68, IsRomanDigit); // C D
    set_mask(67, 68, IsNotLineBegin);

    set_mask(73, 73, IsRomanDigit); // I
    set_mask(73, 73, IsNotLineBegin);

    set_mask(76, 77, IsRomanDigit); // L M
    set_mask(76, 77, IsNotLineBegin);

    set_mask(86, 86, IsRomanDigit); // V
    set_mask(86, 86, IsNotLineBegin);

    set_mask(88, 88, IsRomanDigit); // X
    set_mask(88, 88, IsNotLineBegin);

    set_mask(91, 96, IsPunct);

    set_mask(97, 122, IsAlpha);
    set_mask(97, 122, IsLower);

    set_mask(123, 126, IsPunct);
    set_mask(127, 127, IsStrangeDigit);

    set_mask(128, 167, IsPunct);

    set_mask(133, 133, IsNotLineBegin)

    set_mask(168, 168, IsAlpha);
    set_mask(168, 168, IsUpper);

    set_mask(169, 183, IsPunct);

    set_mask(184, 184, IsAlpha);
    set_mask(184, 184, IsLower);

    set_mask(185, 191, IsPunct);

    set_mask(192, 223, IsAlpha);
    set_mask(192, 223, IsUpper);

    set_mask(224, 255, IsAlpha);
    set_mask(224, 255, IsLower);

}

void my_strcpy (char* dest, const char* s ) {

    *dest++ = ' ';
    while ((*dest++ = *s++) != '\0') ;
}
