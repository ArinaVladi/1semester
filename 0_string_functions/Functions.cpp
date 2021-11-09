#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//-------1---------------------------------------------------------------------

int my_puts (const char* ptr_s) {

    char c = '\0';
    while (c = *ptr_s++) {
        putc(c, stdout);
    }

    return (ferror (stdout))? EOF : 0;
}

//--------2--------------------------------------------------------------------

char* my_strchr (const char* ptr_str, char c) {

    while (*ptr_str && (*ptr_str != c)) {
        ptr_str++;
    }

    return (char *)((c == *ptr_str) ? ptr_str : NULL);
}

//--------3--------------------------------------------------------------------

int my_strlen(const char* ptr_str)  {

    int i = 0;
    while (*ptr_str++ != '\0') {
        ++i;
    }

    return i;
}

//--------4--------------------------------------------------------------------

void my_strcpy (char* dest, const char* s ){

    while ((*dest++ = *s++) != '\0') ;
}

//--------5--------------------------------------------------------------------

 void my_strncpy(char *dest, const char *s, int n)
 {
     while ((n-- > 0) && (*dest++ = *s++));
 }

//--------6--------------------------------------------------------------------

void my_strcat (char* dest, const char* s) {

    while (*++dest);
    while (*dest++ = *s++);
}

//--------7--------------------------------------------------------------------

void* my_strncat (char* dest, const char* s, int n ){

    while (*++dest);
    while ((n-- > 0) && (*dest++ = *s++));
}

//--------8--------------------------------------------------------------------

char* my_fgets (char* dest, int n, FILE* iop) {

    while ((n-- > 0) && (((*dest++ = getc(iop)) != EOF) != '\n' )) ;
    *dest = '\0';

    return (feof(iop)) ? NULL : dest;
}

//--------9--------------------------------------------------------------------

char* my_strdup (const char* s) {

    char* ptr = (char* ) calloc (strlen (s)+1, sizeof(char));

    if (ptr == NULL) {
       return NULL;
    }
    else {
        my_strcpy(ptr, s);
    }

    return ptr;
}

//-------10--------------------------------------------------------------------

int my_getline (char* dest, int max) {
    int i = my_strlen(fgets (dest, max, stdin));

    if (i == NULL) {
        return NULL;
    }
    return i;
}

//-----------------------------------------------------------------------------

int main() {

    int nTest = 1;

    const char* s = "abcd";
//  Test 1 puts
    printf("Test 1: ");
    my_puts(s);
    fflush(stdout);

//  Test 2 strchr
    printf ("\nTest 2: " "%s\n", my_strchr (s, 'a'));
    fflush(stdout);

//  Test 3 strlen
    printf("Test 3: " "%d\n", my_strlen(s));
    fflush(stdout);

//  Test 4 strcpy
    printf("Test 4: ");
    char* ptr = (char*) calloc(100, sizeof(char));
    my_strcpy(ptr, s);
    printf("%s\n", ptr);
    free(ptr);
    fflush(stdout);

//  Test 5 strnpy
    printf("Test 5: ");
    ptr = (char*) calloc(100, sizeof(char));
    int n = 2;
    my_strncpy(ptr, s, n);
    printf("%s\n", ptr);
    free(ptr);
    fflush(stdout);

//  Test 6 strcat
    printf("Test 6: ");
    ptr = (char*) calloc(100, sizeof(char));
    ptr[0] = '_';
    my_strcat (ptr, s);
    printf("%s\n", ptr);
    free(ptr);
    fflush(stdout);

//  Test 7 strncat
    printf("Test 7: ");
    ptr = (char*) calloc(100, sizeof(char));
    ptr[0] = '_';
    my_strncat (ptr, s, 3);
    printf("%s\n", ptr);
    free(ptr);
    fflush(stdout);

//  Test 8 fgets !
    printf("Test 8: ");
    FILE *in_f = fopen("test8.txt", "r");     // "absdefghi"
    ptr = (char*) calloc(100, sizeof(char));
    my_fgets (ptr, 5, in_f);
    printf("%s\n", ptr);
    free(ptr);
    fflush(stdout);

//  Test 9 strdup
    printf("Test 9: ");
    puts(my_strdup(s));
    fflush(stdout);

//  Test 10 getline
    printf("Test 10: ");
    char* dest = (char*) calloc(100, sizeof(char));
    printf("\t %d\n", my_getline(dest, 100));
    fflush(stdout) ;

    return 0;
}
