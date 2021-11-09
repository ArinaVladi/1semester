#ifndef CPU_UTILITIES_H
#define CPU_UTILITIES_H

#include <sys/stat.h>
#include <cstdint>
#include <cassert>

uint32_t get_hash (void *some_data, int size);

bool is_equiv(const void* x, const void* y);

unsigned long get_file_size_fstat (char* f_name);

#endif //CPU_UTILITIES_H
