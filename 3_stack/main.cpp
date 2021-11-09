#include <stdio.h>

#include "stack.h"
#include  "dump.h"

/**
 * It is user's obligation to initialise the Dump's static variable with log_file name
 */
char* dump_t::file_name = "log_file.txt";

int main () {

    Print_Debug ("Start");
    PD_Value ("Debug level is %d\n", PROTECTION_LVL);

    /**
     * Creating the Dump struct, which is universal for any object type
     */
    dump_t theDump     = {};
    theDump.clear_file ();

    /*
     * Creating the stack
     */
    stack_t theStack = {};

    /**
     * User variable to leverage the stack's ability to send signals of errors in methods
     */
    int status = 0;
    
    if ( (status = theStack.Ctor (sizeof (int))) != STACK_OK) {
        method_error (status);
    }

    for (int elem = 1; elem <= 10; elem++) {
        double tmp = elem * 0.5;
        if ( (status = theStack.push (&elem)) != STACK_OK) {
            method_error (status);
        }
    }

    for (int num = 10; num >= 2; num--) {
        double res = 0;
        theStack.top (&res);
        printf ("Stack top before pop = %d\n", res);

        if ( (status = theStack.pop ()) != STACK_OK) {
            method_error (status);
        }

        theStack.top (&res);
        printf ("Stack top after pop = %d\nSize = %d\n", res, theStack.size);
    }

    theStack.Dtor ();

    return 0;
}
