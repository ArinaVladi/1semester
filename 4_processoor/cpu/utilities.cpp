#include "D:\MainFolder\repos\computer3\cpu\utilities.h"

uint32_t get_hash (void *some_data, int size) {

    char* message = (char*) some_data;
    uint32_t mask = 0;
    uint32_t crc = 0x7FFFFFFF;
    for (int n = 0; n < size; n++) {
        uint32_t byte = message[n];
        crc = crc ^ byte;
        for (int j = 7; j >= 0; j--) {
            mask = - (crc & 1);
            crc = (crc >> 1) ^ (0xDEADBEEF & mask);
        }
    }

    return ~crc;
}

unsigned long get_file_size_fstat (char* f_name) {

    assert (f_name != nullptr);

    struct stat file_stat;
    assert (stat (f_name, &file_stat) != -1);
    return file_stat.st_size;
}