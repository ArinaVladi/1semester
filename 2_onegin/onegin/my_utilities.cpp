#include "my_utilities.h"

void my_qsort (void *buf, int nLines, size_t size, int  (*cmp) (const void *, const void *)) {

    assert  (buf != NULL);
    assert  (size > 0);

    //  Print_Debug  ("Начало сортировки");

    unsigned char *ptr =  (unsigned char *) buf;

    while  (nLines > 1) {

        int i = 1, smaller_elements_boarder = 0, greater_elements_boarder = nLines;

        while  (i < greater_elements_boarder) {


            int c = cmp (ptr + smaller_elements_boarder * size, ptr + i * size);

            if  (c > 0) {

                my_swap ( (void*) (ptr + smaller_elements_boarder * size),  (void*) (ptr + i * size), size);
                smaller_elements_boarder++;
                i++;
            } else if  (c < 0) {

                greater_elements_boarder--;
                my_swap ( (void*) (ptr + i * size),  (void*) (ptr + greater_elements_boarder * size), size);

            } else {

                i++;
            }
        }

        if  (smaller_elements_boarder < nLines - greater_elements_boarder) {
            my_qsort (ptr, smaller_elements_boarder, size, cmp);
            ptr += greater_elements_boarder * size;
            nLines -= greater_elements_boarder;
        } else {
            my_qsort (ptr + greater_elements_boarder * size, nLines - greater_elements_boarder, size, cmp);
            nLines = smaller_elements_boarder;
        }
    }
}

int my_cmp_2 (void const *v1, void const *v2, int direction) {

    assert  (v1 != NULL);
    assert  (v2 != NULL);

    const Line *line1 =  (const Line *) v1;
    const Line *line2 =  (const Line *) v2;

    char *s1 = NULL;
    char *s2 = NULL;

    if  (direction > 0) {
        s1 = line1->index;
        s2 = line2->index;
    }
    else {
        s1 = line1->index + line1->length - 1;
        s2 = line2->index + line2->length - 1;
    }


    int length1 = line1->length;
    int length2 = line2->length;

    while  (compare ( (unsigned char) *s1, IsPunct)) {
        s1+=direction;
        length1--;
    }

    while  (compare ( (unsigned char) *s2, IsPunct)) {
        s2+=direction;
        length2--;
    }

    if  (tolower  (*s1) != tolower  (*s2)) {
        return *s1 - *s2;
    }

    while  (tolower  (*s1) == tolower  (*s2)) {
        if  (*s1 == '\0' || *s2 == '\0') {
            return 0;
        }

        else {
            s1 += direction;
            s2 += direction;
        }

    }
    return *s1-*s2;
}


void my_swap (void *first, void *second, size_t size) {

    PD_Value  ("Size is %zu\n", size);

    assert  (first != NULL);
    assert  (second != NULL);
    assert  (size > 0);

    int ost = size % sizeof (uint64_t);
    Print_Debug ("Start swap");
    PD_Value ("Ost = %d\n", ost);
    assert  (ost >= 0 && ost < sizeof (uint64_t));
    if  (ost > 0) {
        if  (ost >= sizeof (uint32_t)) {
            uint32_t tmp = *  (uint32_t*)first;
            *  (uint32_t*)first = *  (uint32_t*)second;
            *  (uint32_t*)second = tmp;
            ost -= sizeof (uint32_t);
            first =  (uint32_t*)first + 4;

        }
        if  (ost >= sizeof (uint16_t)) {
            uint16_t tmp = *  (uint16_t*)first;
            *  (uint16_t*)first = *  (uint16_t*)second;
            *  (uint16_t*)second = tmp;
            ost -= sizeof (uint16_t);
            first =  (uint16_t*)first + 2;
        }
        while  (ost > 0) {
            uint8_t tmp = *  (uint8_t*)first;
            *  (uint8_t*)first = *  (uint8_t*)second;
            *  (uint8_t*)second = tmp;
            ost -= sizeof (uint8_t);
            first =  (uint8_t*)first + 1;
        }
    }
    while  (size > 0) {

        Print_Debug ("size % 8 = 0");

        uint64_t tmp = *  (uint64_t*)first;
        *  (uint64_t*)first = *  (uint64_t*)second;
        *  (uint64_t*)second = tmp;
        ost -= sizeof (uint64_t);
        first =  (uint64_t*)first + 1;
        second =  (uint64_t*)second + 1;
        size -= 8;
    }
}

//-----------------------------------------------------------------------------

/*
int compare  (unsigned char c, int mask) {
    return  ( ( (Symbols[ (unsigned int)c] & mask) == mask) ? 1 : 0);
}
*/

