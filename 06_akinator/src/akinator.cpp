#include "akinator.h"

const char* speech_file_name = "speech_test.txt";

//----------------------------------------------------------------------verification-----------------------------------------------------------------------

char* VERIFY_SIGNALS_NAMES[] = {"Ok",                  // 0
                                "Nodes coinside",      // 1
                                "One node is null"     // 2
                               };

char* END_SIGNALS_NAMES[] = {"Ok",                     // 0
                             "Verification failed",    // 1
                             "Buf alloccation failed", // 2
                            };

#define ASSERT_OK                                    \
        state = verify();                            \
        if (state != END_SIGNALS::OK) {              \
            PRINT_ERROR(state)                       \
        } 
 
#define PRINT_ERROR(err)                             \
        fprintf(log_file,                            \
                "* %s %s\n"                          \
                "* %s -> %s -> %d\n"                 \
                "* ERROR: %s\n\n\n",                 \
                LOCATION,                            \
                END_SIGNALS_NAMES[(int)err]);        \
        fflush(log_file);    

#define PROCESS_ERROR(func)                          \
        verification_state = func;                   \
        if (verification_state != END_SIGNALS::OK) { \
            PRINT_ERROR(verification_state)          \
        } 

END_SIGNALS Tree::verify() {   //?

    VERIFY_SIGNALS tree_state = root->verify();
    if (tree_state != VERIFY_SIGNALS::OK) {
        printf("%s\n", VERIFY_SIGNALS_NAMES[(int)tree_state], stderr);
        return END_SIGNALS::VERIFY_FAILED;
    }
    return END_SIGNALS::OK;
}

VERIFY_SIGNALS Node::verify() {
    if (left == right) {
        if (left == nullptr) {
            return VERIFY_SIGNALS::OK;
        }
        else return VERIFY_SIGNALS::NODES_COINSIDE; 
    }
    else if ((left == nullptr) || (right == nullptr)) {
        return VERIFY_SIGNALS::ONE_NODE_IS_NULL; 
    }

    VERIFY_SIGNALS branch_state = left->verify();
    if (branch_state != VERIFY_SIGNALS::OK) {
        return branch_state;
    }
    else return right->verify();
}

//-----------------------------------------------------------------constructor & destructor----------------------------------------------------------------

END_SIGNALS Tree::Ctor() {

    log_file_name = "log.txt";
    remove(log_file_name);
    log_file = fopen(log_file_name, "w");
    char* buf = (char*) calloc(BUF_SIZE, sizeof(char));
    if (buf == nullptr) {
        return END_SIGNALS::BUF_ALLOCATION_FAILED;
    }
    setvbuf (log_file, buf,_IOFBF, BUF_SIZE);

    root = nullptr;

    return END_SIGNALS::OK;
}

void Node::Ctor(const char* string) {

    left      = nullptr;
    right     = nullptr;
    is_answer =    true;
    is_passed =   false;
    str       =  string;
}

void Tree::Dtor() {

    ASSERT_OK
    root->Dtor();
}

void Node::Dtor() {

    if (left != nullptr) left->Delete();
    if (right != nullptr)right->Delete();
    Delete();
}

void Node::Delete() {
    free(this);
}

//--------------------------------------------------------------------------main---------------------------------------------------------------------------

END_SIGNALS Tree::main() {

    END_SIGNALS verification_state;

    PROCESS_ERROR(Ctor());

    parser.Ctor("tree.txt");

    read_from_buf();
    PROCESS_ERROR(form_tree()); 

    PROCESS_ERROR(choose_mode()); 
    fclose(log_file);

    Dtor();

    return END_SIGNALS::OK;
}

//-------------------------------------------------------------------write tree to file--------------------------------------------------------------------

#define BUF(...) fprintf(buf_file, __VA_ARGS__);

END_SIGNALS Tree::write_to_buf() {

    ASSERT_OK

    FILE* buf_file = fopen("tree.txt", "w");

    char buf[BUF_SIZE] = "";
    setvbuf (buf_file, buf,_IOFBF, BUF_SIZE);
    root->write_to_buf(buf_file);

    fclose(buf_file);

    ASSERT_OK
}

void Node::write_to_buf(FILE* buf_file) {

    BUF("<%s>", str)

    if (left != right) {

        BUF(" {\n")

        if (left != nullptr) {
            left->write_to_buf(buf_file);
        }
        else BUF("\n");

        if (right != nullptr) {
            right->write_to_buf(buf_file);
        }
        else BUF("\n");

        BUF("}\n")
    }

    else BUF("\n"); 
}

//-------------------------------------------------------------------read tree from file-------------------------------------------------------------------

void Tree::read_from_buf() {
    
    parser.Ctor("tree.txt");
    parser.pars();

}

END_SIGNALS Tree::form_tree() {

    int start_token_index = 0;
    root = (Node*)calloc(1, sizeof(Node));
    assert(root != nullptr);
    root->form_tree(&(parser.syntagms), &start_token_index);

    ASSERT_OK
}

#define TOKEN (*syntagms)[*token_num].index
#define FIRST_SYMB (*syntagms)[*token_num].index[0]

void Node::form_tree(syntagm_t** syntagms, int* token_num) {  

    if (isalpha(FIRST_SYMB) || isdigit(FIRST_SYMB)) {
        
        Ctor(TOKEN);
        (*token_num)++;
    }

    if (FIRST_SYMB == '{') {

        (*token_num)++;
        left = (Node*)calloc(1, sizeof(Node));
        assert(left != nullptr);
        left->form_tree(syntagms, token_num);

        if (!(isalpha(FIRST_SYMB) || isdigit(FIRST_SYMB))) {

            (*token_num)++;
            while ((FIRST_SYMB) == '}') {

                (*token_num)++;
            }
        }
        while ((FIRST_SYMB) == '}') {
                (*token_num)++;
        }

        right = (Node*)calloc(1, sizeof(Node));
        assert(right != nullptr);

        right->form_tree(syntagms, token_num);
    }
}

//==========================================================================modes==========================================================================

//-----------------------------------------------------------------------choose mode-----------------------------------------------------------------------

enum class MODES {
    GUESS      = 'g',
    DEFINITION = 'd',
    COMPARISON = 'c',
    SHOW_TREE  = 's',
    ESCAPE     = 'e',
};

END_SIGNALS tree_func_wrapper(Tree* tree, END_SIGNALS (Tree::*func) (void)) {
    END_SIGNALS returned_val = (tree->*func)();
    tree->show_after_another_mode();
}

END_SIGNALS Tree::choose_mode() {

    ASSERT_OK
    
    int user_input;
        while (write_to_speech_file("\nChoose one of five modes. Guess object, get object's definition, make comparison, show tree or escape\n") &&
               printf("Choose mode\n" "[G]uess \\ Get [D]efinition \\ Make [C]omparison \\ [S]how tree \\ [E]scape \n") &&
               ((user_input = tolower(get_user_input()[0])) != (int)MODES::ESCAPE)) {
            
        switch (user_input) {
            
            case (int)MODES::GUESS :
                guess();
                show_after_another_mode();
                break;
    
            case (int)MODES::DEFINITION :
                give_definition();
                show_after_another_mode();
                break;
    
            case (int)MODES::COMPARISON :
                make_comparison();
                show_after_another_mode();
                break;  
    
            case (int)MODES::SHOW_TREE :
                show_tree();
                break;
    
            default:
                write_to_speech_file("Wrong user input.\n");
                assert(0);
        }

        clear_is_passed();

    }
    ASSERT_OK
    return END_SIGNALS::OK;
} 

END_SIGNALS Tree::clear_is_passed() {
    
    ASSERT_OK

    root->clear_is_passed();

    ASSERT_OK

    return END_SIGNALS::OK;
}

void Node::clear_is_passed() {
    
    if (is_passed == true) {
        if (left != right) {
            left->clear_is_passed();
            right->clear_is_passed();
        }
    }
    is_passed = false;
}

//--------------------------------------------------------------------------guess--------------------------------------------------------------------------

END_SIGNALS Tree::guess() {

    ASSERT_OK

    List undefined_answers;
    undefined_answers.Ctor();

    TREE_STATE change_state = root->guess(&undefined_answers);

    ASSERT_OK

    if (change_state == TREE_STATE::CHANGED) {

        write_to_speech_file("Do you want to save changes in tree?");
        
        char user_input = process_Yes_No_answer();

        PROCESS_BINARY_ANSWER(
            { write_to_buf(); },
            {
                return form_tree();
            }
        )
        
    } 
    ASSERT_OK
    return END_SIGNALS::OK;
}


TREE_STATE Node::guess(List* undefined_answers) {
    
    is_passed=true;
    TREE_STATE state = TREE_STATE::UNCHANGED;

    write_to_speech_file("Is it %s?\n", str);
    
    char user_input = process_Yes_No_answer();
    
    PROCESS_YES_NO_ANSWER(
        {state = left->process_node(undefined_answers);},
        {state = right->process_node(undefined_answers);}
    )
    
    return state;
}

TREE_STATE Node::process_node(List* undefined_answers) {

    TREE_STATE state = TREE_STATE::UNCHANGED;

    is_passed = true;
    if (left != right) state = guess(undefined_answers);

    else {
       
        write_to_speech_file("The answer is %s?\n", str);

        char user_input = process_Yes_No_answer();

        PROCESS_YES_NO_ANSWER(
            {write_to_speech_file("Victory!\n");},
            {
                if ((*undefined_answers).size > 0) {
                    Node* returned_node = (Node*) (*undefined_answers).back();
                    (*undefined_answers).pop_back();
                    if (returned_node->right->left == nullptr) {
                        return returned_node->right->process_node(undefined_answers);
                    }
                    else{
                        return (returned_node->right)->guess(undefined_answers); 
                    }                   
                }
                else {
                    printf("No undef unswers!\n");
                    state = TREE_STATE::CHANGED;
                    add_node();
                }
            }
        )
    } 
    return state;
}

void Node::add_node() {

    write_to_speech_file("What are you thinking about?\n");

    char* new_node_str = get_user_input();
    
    write_to_speech_file("How is %s differ from %s?\n", new_node_str, str);

    char* new_deffinition = get_user_input(); 

    left = (Node*)calloc(1, sizeof(Node));
    left->str = new_node_str;

    right = (Node*)calloc(1, sizeof(Node));
    right->str = str;

    str = new_deffinition;
    left->is_passed = true;
}

//-----------------------------------------------------------------------definition------------------------------------------------------------------------

void Definition::Ctor(char* name_to_search) {

    name = name_to_search;
    features = (feature*) calloc(MAX_DEPTH, sizeof(feature)); 
    features_num = 0;   
}

END_SIGNALS Tree::give_definition() {

    ASSERT_OK
    
    printf("Enter object:\n");
    char* name = get_user_input();

    Definition def = {};
    def.Ctor(name);
    root->get_definition(&def); 
    def.print_definition();

    ASSERT_OK
}

Node* Node::get_definition(Definition* def) {
    
    if (left == right) { 

        if (strcmp(str, def->name) == 0) {
            is_passed = true;
        }
        else {
            return nullptr;
        }
    }

    else { 

        Node* tmp_left  =  left->get_definition(def);
        Node* tmp_right = right->get_definition(def);

        if(tmp_left == NULL && tmp_right == NULL) {
            return NULL;
        }

        else {

            if(tmp_left == NULL){
                def->put_feature(str, false);
                def->features_num++;

                return tmp_right;
            }
            else{
                def->put_feature(str, true);
                def->features_num++;
                return tmp_left;
            }
        }
    }
}

#define NAME(num) features[num].feature_name
#define IS(num)   features[num].is_feature

void Definition::put_feature(const char* feature, bool is_feature) {

    NAME(features_num) =    feature;
    IS(features_num)   = is_feature;
}

char* IS_FEATURE[] = {"is not", "is"};

void Definition::print_definition() {

    if (features_num == 0) {
        write_to_speech_file("The name providede is not in base\n");
        return;
    }

    int iter = features_num-1;
    write_to_speech_file("%s ", name);

    for (; iter > 0; iter--) {
         write_to_speech_file("%s %s, ", IS_FEATURE[IS(iter)], NAME(iter));
    }
    
    write_to_speech_file("and %s %s.\n", IS_FEATURE[IS(0)], NAME(0));
}

//-----------------------------------------------------------------------comparison-----------------------------------------------------------------------_

Definition Tree::get_name_for_definition() {

    ASSERT_OK

    Definition def = {};

    char* first = get_user_input();
    def.Ctor(first);
    root->get_definition(&def);

    ASSERT_OK

    return def;
}


END_SIGNALS Tree::make_comparison() {

    ASSERT_OK
    
    char* buf = (char*)calloc(MAX_STR_LEN, sizeof(char));

    write_to_speech_file("Enter first object:\n");
    Definition def1 = get_name_for_definition();
    write_to_speech_file("Enter second object:\n");
    Definition def2 = get_name_for_definition();

    int iter1 = def1.features_num - 1;
    int iter2 = def2.features_num - 1;

    write_to_speech_file("\nEach of them "); 

    while ((strcmp(def1.NAME(iter1), def2.NAME(iter2)) == 0) 
        && (IS_FEATURE[def1.IS(iter1)] == IS_FEATURE[def2.IS(iter2)])
        && (iter1 >= 0)
        && (iter2 > 0)) {
        
        write_to_speech_file("%s %s, ", IS_FEATURE[def1.IS(iter1)], def1.NAME(iter1));
        iter1--;
        iter2--;
    }

    write_to_speech_file("\nbut %s ", def1.name);
    def1.print_comparative_definition(iter1);
    
    write_to_speech_file("while %s ", def2.name);
    def2.print_comparative_definition(iter2);

    ASSERT_OK
}

void Definition::print_comparative_definition(int iter) {
    while (iter >= 1) {
        write_to_speech_file("%s %s, ", IS_FEATURE[IS(iter)], NAME(iter));
        iter--;
    }
    write_to_speech_file("%s %s\n", IS_FEATURE[IS(0)], NAME(0));
}

//------------------------------------------------------------------------show tree------------------------------------------------------------------------

END_SIGNALS Tree::show_tree() {

    ASSERT_OK

    do_dump();

   /* FILE* html_file = fopen("html_tmp.html", "w");

    #define Xprintf(...) fprintf(html_file, __VA_ARGS__);

    Xprintf("%s", "<!doctype html>\n\n"
                             "<style>\n"
                             "    table, th, td {\n"
                             "        border:1px solid black;\n"
                             "    }\n"
                             "</style>\n\n"
                             "<html lang=\"en\">\n"
                             "<head>\n"
                             "    <meta charset=\"utf-8\">\n"
                             "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n\n"
                             "    <title>Akinator tree</title>\n"
                             "</head>\n"
                             "<body>\n\n");

    Xprintf("%s", "<img src=\"tree.png\" alt=\"tree\">\n");

    Xprintf("%s", "</body>\n"
                "</html>\n"); 

    fclose(html_file);

    system("firefox html_tmp.html"); */
    system("xdg-open tree.png"); 

    ASSERT_OK                                
}

END_SIGNALS Tree::show_after_another_mode() {

    ASSERT_OK

    write_to_speech_file("Show tree?");

    char user_input = process_Yes_No_answer();

    PROCESS_BINARY_ANSWER(
        {show_tree();},
        {return END_SIGNALS::OK;}    
    )

    ASSERT_OK
}

//--------------------------------------------------------------------------dump---------------------------------------------------------------------------

#define DUMP(...)  fprintf(dump_file, __VA_ARGS__);

void Node::do_dump(FILE* dump_file) {

    DUMP("[splines=ortho, color=\"#3d5a80\"]")
    DUMP("edge[splines=ortho, color=\"#3d5a80\"]")

    if (left != nullptr) DUMP("%zu -> %zu [label=\"yes\"]\n", (size_t)this, (size_t)left);
    if (right != nullptr) DUMP("%zu -> %zu [label=\"no\"]\n", (size_t)this, (size_t)right);

    const char*  default_color = "#e0fbfc";
    const char* color =  (is_passed)? "#ee6c4d" : ((left == right)? "#98c1d9" : "#e0fbfc");

    DUMP("%zu [ label = <\n"
            "    <table>\n"
            "        <tr><td port=\"Root\" colspan=\"2\" bgcolor=\"%s\">%s</td></tr>\n"
   IF_DEBUG("        <tr><td port=\"adsress\" colspan=\"2\" bgcolor=\"%s\">%p</td></tr>\n")
            "        <tr><td port=\"left\" bgcolor=\"%s\">%s</td><td port=\"right\" bgcolor=\"%s\">%s</td></tr>\n"
   IF_DEBUG("        <tr><td port=\"left_adsress\" bgcolor=\"%s\">%p</td><td port=\"right_address\" bgcolor=\"%s\">%p</td></tr>\n")
            "    </table>\n"
            "> ]\n", (size_t)this, color, str, 
          IF_DEBUG(  (color, this,))
                     (left != nullptr)? ((left->is_passed)? color : default_color) : color, 
                     (left != nullptr)? left->str : "-",  
                     (right != nullptr)? ((right->is_passed)? color : default_color) : color, 
                     (right != nullptr)? right->str : "-"
          IF_DEBUG((,(left != nullptr)? ((left->is_passed)? color : default_color) : color, 
                     left,  
                     (right != nullptr)? ((right->is_passed)? color : default_color) : color, 
                     right)));       


    if (left != nullptr) left->do_dump(dump_file);
    if (right != nullptr) right->do_dump(dump_file);
}

END_SIGNALS Tree::do_dump() {
    
    ASSERT_OK

    remove("tree.gz");
    FILE* dump_file = fopen("tree.gz", "a");
    DUMP("digraph G {\n"
           "    rankdir=TB\n"
           "    node [shape=plaintext]\n");   
    
    root->do_dump(dump_file);
    DUMP("}");
    fclose(dump_file);
    system("dot -q -Tpng tree.gz -o tree.png");

    ASSERT_OK
}

//------------------------------------------------------------------------auxilary-------------------------------------------------------------------------

char process_Yes_No_answer() {

    printf("\nYes \ IDK \ No\n"
             " 1     2     3\n");

    char user_input = (get_user_input()[0]);
    
    return user_input;
}

char* get_user_input() { 

    char* buf = (char*)calloc(MAX_STR_LEN, sizeof(char)); 
    get_line(buf, sizeof(buf));                           
    char* user_input = (char*)calloc(MAX_STR_LEN, sizeof(char));   
    sscanf(buf , "%[^\t\n]", user_input);  

    free(buf);

    return user_input;
}

int write_to_speech_file(char* format, ...) { 

    FILE* speech_file = fopen(speech_file_name, "w+");
   
    va_list arg_list;
    va_start(arg_list, format);

    int length = strlen(format);   
    for (int cur_len = 0; cur_len <= length; cur_len++) {     

        while ((format[cur_len] != '%') && (format[cur_len]!= '\0')) {    
            fprintf(speech_file, "%c", format[cur_len]);
            putc(format[cur_len], stdout);
            cur_len++;
        }

        if (format[cur_len] != '\0') {cur_len++;}
        
        if (format[cur_len] != '\0') {

            switch (format[cur_len]) {
                
                case 's': {
                    char* str = va_arg(arg_list, char*);
                    fputs(str, speech_file);
                    fputs(str, stdout);
                    break;
                }    
                default:
                    assert(0 && "Unexpected \"my printf\" parameter\n");    
            }
        }
    } 
    
    va_end(arg_list);

    fflush(stdout);
    fflush(speech_file);

    fclose(speech_file);

    system("festival --tts speech_test.txt"); // переделать с именем файла и буфером

    return 1;
}
