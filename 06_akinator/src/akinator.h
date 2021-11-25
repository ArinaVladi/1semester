#include  <stdio.h>
#include <stdlib.h>
#include  <cassert>
#include <typeinfo>
#include  <cstring>
#include <stdarg.h>

#include "parser.h"
#include "list/list.h"

//--------------------------------------------------------------------------debug--------------------------------------------------------------------------

#define D printf("%s -> %s -> %d\n", __FILE__, __func__, __LINE__); 

#define LOCATION __DATE__, __TIME__, __FILE__, __func__, __LINE__ 

// #define DEBUG

#if defined DEBUG
    #define IF_DEBUG(code) code
#else
    #define IF_DEBUG(code)
#endif //DEBUG  

enum class VERIFY_SIGNALS {

    OK               = 0,
    NODES_COINSIDE   = 1,
    ONE_NODE_IS_NULL = 2,
};

enum class END_SIGNALS {
    OK = 0,
    VERIFY_FAILED = 1,
    BUF_ALLOCATION_FAILED = 2,
};

//-------------------------------------------------------------------------auxilary------------------------------------------------------------------------

const size_t MAX_STR_LEN = 300;
const size_t MAX_DEPTH   = 100;

const int BUF_SIZE = MAX_STR_LEN * MAX_DEPTH;

typedef struct {
    const char* feature_name;
    bool    is_feature;
} feature;

typedef struct {

    int    features_num;
    char*          name;

    feature*   features;
    
    void    Ctor(char* name_to_search);
    void    put_feature(const char* feature, bool is_feature);
    void    print_definition();
    void    print_comparative_definition(int iter);
} Definition;

//----------------------------------------------------------------------tree and node----------------------------------------------------------------------

enum class TREE_STATE {
 
    UNCHANGED = 0,
    CHANGED   = 1,
};

typedef struct Node_t {

    Node_t*   parent;
    Node_t*     left;
    Node_t*    right;

    const char*  str;

    bool   is_answer;
    bool   is_passed;

    void           Ctor(const char* string);
    VERIFY_SIGNALS verify();
    void           Delete();
    void           Dtor();
    
    void           form_tree(syntagm_t** syntagms, int* syntagm_num);
    
    TREE_STATE     guess(List* undefined_answers);
    TREE_STATE     process_node(List* undefined_answers);  
    void           add_node();   
    
    Node_t*        get_definition(Definition* def);
    void           make_comparison();
    
    void           do_dump(FILE* dump_file);

    void           clear_is_passed();

    void           write_to_buf(FILE* buf_file);

} Node;

typedef struct {

    Node*          root;
    Parser       parser; 
    END_SIGNALS   state;  

    char* log_file_name;
    FILE*      log_file;

    END_SIGNALS main();
    END_SIGNALS Ctor();
    END_SIGNALS verify();
    void        Dtor();

    END_SIGNALS write_to_buf();
    void        read_from_buf();
    END_SIGNALS form_tree();
    
    END_SIGNALS choose_mode();

    END_SIGNALS guess();      
    
    Definition  get_name_for_definition();
    END_SIGNALS give_definition();

    END_SIGNALS make_comparison();

    END_SIGNALS show_tree();
    END_SIGNALS show_after_another_mode();

    END_SIGNALS clear_is_passed();

    END_SIGNALS do_dump();

} Tree;

//-----------------------------------------------------------------communication with user-----------------------------------------------------------------

char  process_Yes_No_answer();
char* get_user_input();
int   write_to_speech_file(char* format, ...);

#define PROCESS_YES_NO_ANSWER(yes_action, no_action)  \
    if (user_input == '1') {                          \
        yes_action                                    \
    }                                                 \
    else if (user_input == '3') {                     \
        no_action                                     \
    }                                                 \
    else if (user_input == '2') {                     \
        int* node_ptr = (int*) this;                  \
        undefined_answers->push_back(&node_ptr);      \
        yes_action                                    \
    }                                                 \
    else {                                            \
        printf ("Wrong user input\n");                \
        assert(0);                                    \
    } 
 
#define PROCESS_BINARY_ANSWER(yes_action, no_action)  \
    if ((user_input == '1') || (user_input == '2')) { \
        yes_action                                    \
    }                                                 \
    else if (user_input == '3') {                     \
        no_action                                     \
    }                                                 \
    else {                                            \
        printf ("Wrong user input\n");                \
        assert(0);                                    \
    }    