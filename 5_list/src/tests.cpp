#include <gtest/gtest.h>

// #include "list.h"
#include "test.h"

const int NUM_OF_TESTS = 2000;

static int test_num = 1; 

int main(int argc, char *argv[])
{
    srand(time(NULL));
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

/* int main() {

    List list{};
    list.Ctor();

    any_list_func_ptr some_func;    

//----------Test 1 PUSH BACK----------

  /*  elem_t val = 1.1;
    some_func.elem_func_ptr = &List::push_back;
    func_wrapper(&list, some_func, FUNC_ARG_TYPES::ELEM, 0, &val);
    val = 2.1;

    some_func.elem_func_ptr = &List::push_front;
    func_wrapper(&list, some_func, FUNC_ARG_TYPES::ELEM, 0, &val);
    val = 3.1;

    some_func.elem_func_ptr = &List::push_back;
    func_wrapper(&list, some_func, FUNC_ARG_TYPES::ELEM, 0, &val);
    val = 4.1;

    some_func.elem_func_ptr = &List::push_front;
    func_wrapper(&list, some_func, FUNC_ARG_TYPES::ELEM, 0, &val); 

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
}    */      

static char* test_names[NUM_OF_TESTS];

RETURNED_SIGNALS func_wrapper(char*        test_name,
                              List*             list, 
                              any_list_func_ptr func, 
                              FUNC_ARG_TYPES    type,
                              int              index,
                              const elem_t* elem_ptr) {

    test_names[test_num-1] = test_name;                            

    RETURNED_SIGNALS returned_val;   
    list->dump_list(test_num, 1);
    switch(type) {
        case FUNC_ARG_TYPES::BOTH: {
            returned_val = (list->*func.both_func_ptr)(index, elem_ptr);
            break;  
        }
        case FUNC_ARG_TYPES::INDEX: {
            returned_val = (list->*func.index_func_ptr)(index);
            break;  
        }
        case FUNC_ARG_TYPES::ELEM: {
            returned_val = (list->*func.elem_func_ptr)(elem_ptr); 
            break;
        }
        case FUNC_ARG_TYPES::NO: {
            returned_val = (list->*func.no_func_ptr)();
            break;  
        }         
    }
    list->dump_list(test_num, 2);  
    test_num++; 
    return returned_val;
}   

void do_html(int num_of_tests) {

    FILE* html_file = fopen("dump.html", "w");

    Xprint_html("<!doctype html>\n\n"
                "<style>\n"
                "    table, th, td {\n"
                "        border:1px solid black;\n"
                "    }\n"
                "</style>\n\n"
                "<html lang=\"en\">\n"
                "<head>\n"
                "    <meta charset=\"utf-8\">\n"
                "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n\n"
                "    <title>List DUMP</title>\n"
                "</head>\n"
                "<body>\n\n");

    for (int i = 1; i <= num_of_tests; i++) {
        
        Xprintf_html("    <h1 style=\"color:black;background-color:#F9E79F;text-align:center;\">Test %s</h1>\n"
                     "    <p> </p>\n"
                     "    <p style=\"background-color:#FCF9E7;text-align:center;\">Before</p>\n"
                     "    <img src=\"imgs/table_%d_1.png\" alt=\"table\">\n"
                     "    <p> </p>\n"
                     "    <img src=\"imgs/test_%d_1.png\" alt=\"before\">\n"
                     "    <p> </p>\n"
                     "    <p style=\"background-color:#FCF9E7;text-align:center;\">After</p>\n"
                     "    <img src=\"imgs/table_%d_2.png\" alt=\"table\">\n"
                     "    <p> </p>\n"
                     "    <img src=\"imgs/test_%d_2.png\" alt=\"after\">\n"
                     "    <p> </p>\n"
                     "    <p> </p>\n"
                     "    <p> </p>\n",
                    
                    test_names[i-1], i, i, i, i);
    }
    Xprint_html("</body>\n"
                "</html>\n");
    fclose(html_file);
    system("firefox dump.html");
}


class PushTest : public ::testing::Test {
    protected:

        List list = {};
        elem_t val = 0;
        any_list_func_ptr some_func; 

        void SetUp() {
            list.Ctor();  
        }
           
        void TearDown() {
            list.Dtor();  
        }
};

class InsertTest : public ::testing::Test {
    protected:
    
        List list = {};
        elem_t val = 0;
        any_list_func_ptr some_func; 

        void SetUp() {
            list.Ctor();
            for (int i = 1; i <= 7; i++) {
                val = i * 3.5;
                list.push_front(&val);
            }  
        }
           
        void TearDown() {
            list.Dtor();  
        }
};

class EraseTest : public ::testing::Test {
    protected:
    
        List list = {};
        elem_t val = 0;
        any_list_func_ptr some_func; 

        void SetUp() {
            list.Ctor();
            for (int i = 1; i <= 19; i++) {
                val = i * -2.0;
                list.push_back(&val);
            }  
        }
           
        void TearDown() {
            list.Dtor();  
        }
};

//----------------------------------------Test 1 PUSH BACK----------------------------------------
TEST_F(PushTest, push_back) {
    
    some_func.elem_func_ptr = &List::push_back;
    val = 1.1;
    EXPECT_EQ(func_wrapper("1. Push back", &list, some_func, FUNC_ARG_TYPES::ELEM, 0, &val), RETURNED_SIGNALS::OK);
    EXPECT_DOUBLE_EQ(list.back(), val);
}

//----------------------------------------Test 2 PUSH FRONT----------------------------------------
TEST_F(PushTest, push_front) {
    
    some_func.elem_func_ptr = &List::push_front;
    val = 2.1;
    EXPECT_EQ(func_wrapper("2. Push front", &list, some_func, FUNC_ARG_TYPES::ELEM, 0, &val), RETURNED_SIGNALS::OK);
    EXPECT_DOUBLE_EQ(list.front(), val);
}

//----------------------------------------Test 3 PUSH BACK + INCREASE----------------------------------------
TEST_F(PushTest, push_back_and_increase) {
    
    some_func.elem_func_ptr = &List::push_back;
    list.dump_list(test_num, 1);
    for (int i = 12; i <= 32; i++) {
        val = i * -1.1;
        EXPECT_EQ(func_wrapper("3. Push back and increase", &list, some_func, FUNC_ARG_TYPES::ELEM, 0, &val), RETURNED_SIGNALS::OK);
    }
}

//----------------------------------------Test 4 PUSH FRONT + INCREASE----------------------------------------
TEST_F(PushTest, push_front_and_increase) {
    
    some_func.elem_func_ptr = &List::push_front;
    list.dump_list(test_num, 1);
    for (int i = 12; i <= 32; i++) {
        val = i * 1.1;
        EXPECT_EQ(func_wrapper("4. Push front and increase", &list, some_func, FUNC_ARG_TYPES::ELEM, 0, &val), RETURNED_SIGNALS::OK);
    }
    list.dump_list(test_num, 2);
}

//----------------------------------------Test 5 INSERT HEAD----------------------------------------
TEST_F(InsertTest, insert_head) {
    
    some_func.both_func_ptr = &List::insert;
    val = 1.1;
    EXPECT_EQ(func_wrapper("5. Insert head", &list, some_func, FUNC_ARG_TYPES::BOTH, 0, &val), RETURNED_SIGNALS::OK);
    EXPECT_DOUBLE_EQ(list.front(), val);
}

//----------------------------------------Test 6 INSERT TAIL----------------------------------------
TEST_F(InsertTest, insert_tail) {
    
    some_func.both_func_ptr = &List::insert;
    val = 2.2;
    EXPECT_EQ(func_wrapper("6. Insert tail", &list, some_func, FUNC_ARG_TYPES::BOTH, list.tail, &val), RETURNED_SIGNALS::OK);
    EXPECT_DOUBLE_EQ(list.back(), val);
}

//----------------------------------------Test 7 INSERT WRONG INDEX----------------------------------------
TEST_F(InsertTest, insert_wrong_index) {
    
    some_func.both_func_ptr = &List::insert;
    val = 2.2;
    EXPECT_EQ(func_wrapper("7. Insert wrong index", &list, some_func, FUNC_ARG_TYPES::BOTH, -10, &val), RETURNED_SIGNALS::WRONG_INDEX);
    EXPECT_EQ(func_wrapper("7. Insert wrong index", &list, some_func, FUNC_ARG_TYPES::BOTH, 120, &val), RETURNED_SIGNALS::WRONG_INDEX);
} 

//----------------------------------------Test 8 INSERT BY LOGICAL INDEX----------------------------------------
TEST_F(InsertTest, insert_by_logical_index) {
    
    some_func.both_func_ptr = &List::insert_by_logical_index;
    val = 12.34;
    EXPECT_EQ(func_wrapper("8. Insert by logical index", &list, some_func, FUNC_ARG_TYPES::BOTH, 1, &val), RETURNED_SIGNALS::OK);
    double v1, v2;
    list.data_by_physical_index(8, &v1);
    list.data_by_logical_index(2, &v2);
    EXPECT_DOUBLE_EQ(v1, v2);
    EXPECT_EQ(func_wrapper("8. Insert by logical index", &list, some_func, FUNC_ARG_TYPES::BOTH, 6, &val), RETURNED_SIGNALS::OK);
    list.data_by_physical_index(9, &v1);
    list.data_by_logical_index(7, &v2);
    EXPECT_DOUBLE_EQ(v1, v2);
} 

//----------------------------------------Test 9 INSERT BY PHYSICAL INDEX----------------------------------------
TEST_F(InsertTest, insert_by_physical_index) {
    
    some_func.both_func_ptr = &List::insert;
    val = 12.34;
    EXPECT_EQ(func_wrapper("9. Insert in the middle by physical index", &list, some_func, FUNC_ARG_TYPES::BOTH, 5, &val), RETURNED_SIGNALS::OK);
    double v1, v2;
    list.data_by_physical_index(8, &v1);
    EXPECT_DOUBLE_EQ(v1, val); 
    
} 

//----------------------------------------Test 10 POP BACK + DECREASE----------------------------------------
TEST_F(EraseTest, pop_back) {
    
    some_func.no_func_ptr = &List::pop_back;
    int save_capacity = list.capacity;
    for (int i = 1; i <= 12; i++) {
        EXPECT_EQ(func_wrapper("10. Pop back and decrease", &list, some_func, FUNC_ARG_TYPES::NO, 5, &val), RETURNED_SIGNALS::OK);
    }
    EXPECT_LT(list.capacity, save_capacity); 
} 

//----------------------------------------Test 11 POP FRONT + DECREASE----------------------------------------
TEST_F(EraseTest, pop_front) {
    
    some_func.no_func_ptr = &List::pop_front;
    int save_capacity = list.capacity;
    for (int i = 1; i <= 10; i++) {
        EXPECT_EQ(func_wrapper("11. Pop front and decrease", &list, some_func, FUNC_ARG_TYPES::NO, 5, &val), RETURNED_SIGNALS::OK);
    }
    EXPECT_LT(list.capacity, save_capacity); 
}

//----------------------------------------Test 12 ERASE + DECREASE----------------------------------------
TEST_F(EraseTest, erase) {
    
    some_func.index_func_ptr = &List::erase;
    int save_capacity = list.capacity;
    for (int i = 18; i >= 5; i--) {
        EXPECT_EQ(func_wrapper("12. Erase and not decrease", &list, some_func, FUNC_ARG_TYPES::INDEX, i, &val), RETURNED_SIGNALS::OK);
    }
    EXPECT_EQ(list.capacity, save_capacity); 
}

//----------------------------------------Test 13 ERASE + DECREASE----------------------------------------
TEST_F(EraseTest, erase_wrong_index) {
    
    some_func.index_func_ptr = &List::erase;
    int save_capacity = list.capacity;
    EXPECT_EQ(func_wrapper("13. Erase and not decrease", &list, some_func, FUNC_ARG_TYPES::INDEX, -1, &val), RETURNED_SIGNALS::WRONG_INDEX);
    EXPECT_EQ(func_wrapper("13. Erase and not decrease", &list, some_func, FUNC_ARG_TYPES::INDEX, 120, &val), RETURNED_SIGNALS::WRONG_INDEX);
    
    for (int i = 18; i >= 5; i--) {
        list.erase(i);
    }
    EXPECT_EQ(func_wrapper("13. Erase and not decrease", &list, some_func, FUNC_ARG_TYPES::INDEX, 17, &val), RETURNED_SIGNALS::WRONG_INDEX);
    
    EXPECT_EQ(list.capacity, save_capacity); 
}

//----------------------------------------Test _ FINAL TEST----------------------------------------
TEST(DoHTML, DoHTML) {
    do_html(test_num-1);
}
