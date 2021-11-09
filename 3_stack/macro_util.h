#ifndef MACROUTILITIES_H
#define MACROUTILITIES_H

#include "dump.h"

//----------------------------------------user "fast" defines----------------------------------------

//#define DEBUG
#define ALWAYS_DUMP
#define TOTAL_PROTECTION
//#define PROTECTION_LVL 3


#if defined TOTAL_PROTECTION
    #undef  PROTECTION_LVL
    #define PROTECTION_LVL 100
#endif //TOTAL_PROTECTION


//----------------------------------------defines for levels of protection----------------------------------------

#undef PRINTF_CHECK
#undef VERIFIER
#undef IF_VERIFIER
#undef HASH_PROTECTION
#undef IF_HASH_PROTECTION
#undef CANARY_PROTECTION
#undef IF_CANARY_PROTECTION

#if defined DEBUG
    #define PRINTF_CHECK
#endif //DEBUG

#if (PROTECTION_LVL >= 1)
    #define VERIFIER
#endif //(PROTECTION_LVL >= 1)

#if (PROTECTION_LVL >= 2)
    #define HASH_PROTECTION
#endif //(PROTECTION_LVL >= 2)

#if (PROTECTION_LVL >= 3)
    #undef HASH_PROTECTION
    #define CANARY_PROTECTION
#endif //(PROTECTION_LVL >= 3)

#if (PROTECTION_LVL >= 4)
    #define HASH_PROTECTION
    #define CANARY_PROTECTION
#endif //(PROTECTION_LVL >= 4)

#if defined HASH_PROTECTION
    #define IF_HASH_PROTECTION(code) code
#else
    #define IF_HASH_PROTECTION(code)
#endif //HASH_PROTECTION

#if defined CANARY_PROTECTION
    #define IF_CANARY_PROTECTION(code) code
#else
    #define IF_CANARY_PROTECTION(code)
#endif //CANARY_PROTECTION

#if defined PRINTF_CHECK
    #define Print_Debug(s, ...) printf ("%s\n" "File -%s-, function -%s-, line -%d- \n\n", s, __FILE__, __func__, __LINE__ __VA_ARGS__);
    #define PD_Value(...) printf (__VA_ARGS__);
#else
    #define Print_Debug(s)
    #define PD_Value(...)
#endif //PRINTF_CHECK

#if defined VERIFIER

//----------------------------------------print_dump_impl defines----------------------------------------

    #define IS_OK(condition) (condition)? "(error)" : "(ok)"
    #define IS_POISON(elem) (elem == INT_POISON)? "(poison)" : "(ok)"
    #define IS_INIT(elem) (elem == INT_POISON)? "-" : "+"
    #define IS_MODE (PROTECTION_LVL >= 1)?                                                                                                         \
                        ((PROTECTION_LVL >= 2)?                                                                                                    \
                            ((PROTECTION_LVL >= 3)?                                                                                                \
                                ((PROTECTION_LVL >= 4)?                                                                                            \
                                    ((PROTECTION_LVL >= 5)?                                                                                        \
                                        "\tVerifier           +\n\tHash protection    +\n\tCanary protection  +\n\tCopy protection    -\n"         \
                                    :"\tVerifier           +\n\tHash protection    +\n\tCanary protection  +\n\tCopy protection    -\n")           \
                                : "\tVerifier           +\n\tHash protection    +\n\tCanary protection  -\n\tCopy protection    -\n")              \
                            : "\tVerifier           +\n\tHash protection    -\n\tCanary protection  -\n\tCopy protection    -\n")                  \
                       : "\tPrint_Debug only\n")                                                                                                   \
                    : "\tRelease mode\n"

//----------------------------------------assert ok defines----------------------------------------

    #define IF_VERIFIER(code) code

    #if defined ALWAYS_DUMP
        #define IF_ALWAYS_DUMP(code) code
        #define IF_NOT_ALWAYS_DUMP(code)
    #else
        #define IF_ALWAYS_DUMP(code)
        #define IF_NOT_ALWAYS_DUMP(code) code
    #endif //ALWAYS_DUMP
        #define ASSERT_OK                                                                                  \
        if ((stackOK()) IF_ALWAYS_DUMP(|| 1)) {                                                            \
            int error = stackOK();                                                                         \
            dump_t theDump = {};                                                                             \
            theDump.initialize(this, LOCATION, "0", elem_size);                                            \
            theDump.print_dump_impl(this, print_dump_wrapper_void_to_stack, get_error_name(error), error); \
            IF_NOT_ALWAYS_DUMP(printf("Sth went wrong. Check log."));                                      \
        }
#else //VERIFIER
    #define IF_VERIFIER(code)
    #define ASSERT_OK
#endif //VERIFIER

#endif //MACROUTILITIES_H

