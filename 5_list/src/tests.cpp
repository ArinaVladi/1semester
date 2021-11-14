// #include <gtest/gtest.h>

#include "list.h"
#include "test.h"

static int test_num = 1; 

int main() {

    List list{};
    list.Ctor();

    any_list_func_ptr some_func;    

//----------Test 1 PUSH BACK----------

    elem_t val = 1.1;
    some_func.elem_func_ptr = &List::push_back;
    for (int i = 12; i <= 17; i++) {
        elem_t val = i * 1.1;
        func_wrapper(&list, some_func, FUNC_ARG_TYPES::ELEM, 0, &val);
    }

//----------Test 2 PUSH FRONT---------

    some_func.elem_func_ptr = &List::push_front;
    for (int i = 12; i <= 22; i++) {
        elem_t val = i * -1.1;
        func_wrapper(&list, some_func, FUNC_ARG_TYPES::ELEM, 0, &val);
    }    

//------------Test 3 ERASE------------ 

    some_func.index_func_ptr = &List::erase;
    func_wrapper(&list, some_func, FUNC_ARG_TYPES::INDEX, 10, &val);

//----Test 4 ERASE BY LOGICAL INDEX----    

    some_func.index_func_ptr = &List::erase_by_by_logical_index;
    func_wrapper(&list, some_func, FUNC_ARG_TYPES::INDEX, 3, &val);    

//------------Test 5 INSERT------------  

    some_func.both_func_ptr = &List::insert;
    val = 12.34;
    func_wrapper(&list, some_func, FUNC_ARG_TYPES::BOTH, 4, &val); 

//----Test 6 INSERT BY LOGICAL INDEX----  

    some_func.both_func_ptr = &List::insert;
    val = 43.21;
    func_wrapper(&list, some_func, FUNC_ARG_TYPES::BOTH, 4, &val); 

//----------Test 7 POP BACK-----------

    some_func.no_func_ptr = &List::pop_back;
    func_wrapper(&list, some_func, FUNC_ARG_TYPES::NO, 1, &val);

//----------Test 8 POP FRONT----------

    some_func.no_func_ptr = &List::pop_front;
    func_wrapper(&list, some_func, FUNC_ARG_TYPES::NO, 1, &val);  

//-------------Test 9 SORT------------ 

    some_func.no_func_ptr = &List::sort;
    func_wrapper(&list, some_func, FUNC_ARG_TYPES::NO, 1, &val);  

    list.Dtor();

    list.do_html(test_num-1);
}          

void func_wrapper(List*             list, 
                  any_list_func_ptr func, 
                  FUNC_ARG_TYPES    type,
                  int              index,
                  const elem_t* elem_ptr) {

    list->dump_list(test_num, 1);
    switch(type) {
        case FUNC_ARG_TYPES::BOTH: {
            (list->*func.both_func_ptr)(index, elem_ptr);
            break;  
        }
        case FUNC_ARG_TYPES::INDEX: {
            (list->*func.index_func_ptr)(index);
            break;  
        }
        case FUNC_ARG_TYPES::ELEM: {
            (list->*func.elem_func_ptr)(elem_ptr); 
            break;
        }
        case FUNC_ARG_TYPES::NO: {
            // no_arg_func_ptr _new_func_ = (no_arg_func_ptr) func;
            (list->*func.no_func_ptr)();
            break;  
        }         
    }
    list->dump_list(test_num, 2);  
    test_num++; 
}               