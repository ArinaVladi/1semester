#pragma once

#include <stdio.h>
#include <cassert>
#include <cstring>
#include <stdlib.h>
#include "constants.h"

typedef double elem_t;

typedef struct {

    elem_t data;
    int    next;
    int    prev;
} Node;

typedef struct {

    Node* elements;
    int   capacity;
    int       head;
    int       tail;
    int       first_free;
    bool is_sorted;
    int     status;

    void          Ctor();
    void          Dtor();

    elem_t*       front();
    elem_t*       back();

    bool          empty();
    unsigned int  size();

    void          increase_capacity();

    void          insert(int index, const elem_t* value);

    void          push_back(const elem_t* value);
    void          push_front(const elem_t* value);

    void          dump(int test_num, int pass);
    void          do_html(int num_of_tests);
} List;