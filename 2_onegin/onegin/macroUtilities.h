#ifndef ONEGIN_ONE_FILE_MACROUTILITIES_H
#define ONEGIN_ONE_FILE_MACROUTILITIES_H

#define DEBUG
//#undef DEBUG

#ifdef DEBUG
#define Print_Debug(s, ...) printf ("%s\n" "File -%s-, functon -%s-, line -%d- \n\n", s, __FILE__, __func__, __LINE__ __VA_ARGS__);
#define PD_Value(...) printf (__VA_ARGS__);
#else
#define Print_Debug(s)
#define PD_Value(...)
#endif

#endif //ONEGIN_ONE_FILE_MACROUTILITIES_H
