#include "list.h"

const char* ERROR_NAMES[] = {
    "              OK.",                                 // 0
    "              zero element is wrong;\n",            // 1
    "              one of \"next\" fields is wrong;\n",  // 2
    "              one of \"prev\" fields is wrong;\n",  // 3
    "              some of size parametres are wrong;\n" // 4
};

char* END_SIGNALS[] = {
    "OK",
    "ERROR"
};

enum END_SIGNALS_NAMES {
    OK    = 0,
    ERROR = 1
};

RETURNED_SIGNALS List::Ctor() {

    capacity = CAPACITY;
    elements = (Node*) calloc(capacity, sizeof(Node));
    if (elements == nullptr) {
        return RETURNED_SIGNALS::OK;
    }
    head       =    0;
    tail       =    0;
    first_free =    1;
    is_sorted  = true;
    status     =    0;

    log_file_name = "log_file.txt";
    remove("log_file.txt");

    for (int i = 0; i < capacity; i++) {
        //               data,  next, prev 
        elements[i] = {POISON,  i+ 1,   -1};
    }

    elements[0] = {POISON, 0, 0};

    ASSERT_OK

    return RETURNED_SIGNALS::OK;
}

RETURNED_SIGNALS List::Dtor() {

    ASSERT_OK

    for (int i = 0; i < capacity; i++) {
        //               data, next, prev 
        elements[i] = {POISON,   -1,   -1};
    }

    capacity = head = tail = first_free = status = INT_POISON;
    is_sorted  = false;
    
    free(elements);
    return RETURNED_SIGNALS::OK;
}

//--------------------------------------------------------------list size functions---------------------------------------------------------------


elem_t List::front() {
    ASSERT_OK
    return elements[head].data; 
}

elem_t List::back() {
    ASSERT_OK
    return elements[tail].data; 
}

RETURNED_SIGNALS  List::increase_capacity() {

    ASSERT_OK

    int increased_capacity = 2 * capacity;
    Node* increased_elements = (Node*) calloc(increased_capacity, sizeof(Node));
    if (increased_elements == nullptr) {
        return RETURNED_SIGNALS::INCREASE_FAILURE;
    }

    for (int i = 0; i < capacity; i++) {
        increased_elements[i].data = elements[i].data;
        increased_elements[i].prev = elements[i].prev;
        increased_elements[i].next = elements[i].next;
    }

    for (int i = capacity; i < increased_capacity; i++) {
        increased_elements[i].data = POISON;
        increased_elements[i].prev =     -1;
        increased_elements[i].next =    i+1;
    }

    capacity = increased_capacity;

    free(elements);

    elements = increased_elements;
    ASSERT_OK
    return RETURNED_SIGNALS::OK;
}

RETURNED_SIGNALS  List::decrease_capacity() {

    ASSERT_OK

    if (is_sorted == false) {
        return RETURNED_SIGNALS::WRONG_IS_SORTED_STATE;
    }

    int decreased_capacity = capacity / 2;
    Node* decreased_elements = (Node*) calloc(decreased_capacity, sizeof(Node));
    if (decreased_elements == nullptr) {
        return RETURNED_SIGNALS::DECREASE_FAILURE;
    }
    
    size_t curr = 0;
    
    for(int i = 0; i <= size; i++){
        decreased_elements[i].data = elements[i].data;
        decreased_elements[i].prev = elements[i].prev;
        decreased_elements[i].next = elements[i].next;
    }

    for(int i = size + 1; i < decreased_capacity; i++){
        decreased_elements[i].prev = -1;
        decreased_elements[i].next = i+1;
    }
    
    capacity = decreased_capacity;
    free(elements);
    elements = decreased_elements;

    ASSERT_OK

    return RETURNED_SIGNALS::OK;
}

//-------------------------------------------------------------list insert functions--------------------------------------------------------------

RETURNED_SIGNALS List::insert(size_t index, const elem_t* value) {

    ASSERT_OK

    if (((tail == capacity - 1) || (size >= capacity - 1)) && first_free == capacity) {
        if (increase_capacity() != RETURNED_SIGNALS::OK) {
            return RETURNED_SIGNALS::INCREASE_FAILURE;
        }
    }

    if((index > capacity) || (elements[index].prev == -1)){
        return RETURNED_SIGNALS::WRONG_INDEX;
    }

    if (index != tail) is_sorted = false;
    
    elements[first_free].data = *value;
    
    size_t new_first_free = elements[first_free].next;
    
    elements[first_free].prev = index;
    elements[first_free].next = elements[index].next;

    elements[elements[first_free].next].prev = first_free;
    elements[elements[first_free].prev].next = first_free;
    first_free = new_first_free;

    head = elements[0].next;
    tail = elements[0].prev;
    size++;

    ASSERT_OK

    return RETURNED_SIGNALS::OK;
}

RETURNED_SIGNALS List::insert_by_logical_index (size_t logical_index, const elem_t* val) {

    ASSERT_OK

    size_t phys_index = 0;
    get_physical_index_by_logical_index(logical_index, &phys_index);
    insert(phys_index, val);

    ASSERT_OK
}

RETURNED_SIGNALS List::push_back(const elem_t* value) {

    ASSERT_OK

    return insert(tail, value);
}

RETURNED_SIGNALS List::push_front(const elem_t* value) {
    ASSERT_OK
    return insert(0, value);
}

//----------------------------------------------------------list erese and pop functions----------------------------------------------------------

RETURNED_SIGNALS List::erase(size_t index) {

    ASSERT_OK

    if((index == 0) || (index >= capacity) || (elements[index].prev == -1)){
        return RETURNED_SIGNALS::WRONG_INDEX;
    }

    if((index != tail) && (index != head)){
        is_sorted = false;
    }

    elements[elements[index].next].prev = elements[index].prev;
    elements[elements[index].prev].next = elements[index].next;

    if (index < first_free) {
        elements[index].next = first_free;
        first_free = index;
    }
    else {
        int i_prev = first_free;
        for (int i = first_free; i <= capacity; i = elements[i].next) {
            if (index < i) {
                elements[index].next = i;
                elements[i_prev].next = index;
                break;
            }
            i_prev = i;
        }
    }
    
    elements[index].prev =     -1;
    elements[index].data = POISON;
    
    head = elements[0].next;
    tail = elements[0].prev;
    size--;

    if((size < (capacity / 2)) && (is_sorted == true)) {

        if (decrease_capacity() != RETURNED_SIGNALS::OK) {
            return RETURNED_SIGNALS::DECREASE_FAILURE;
        }
    }

    ASSERT_OK

    return RETURNED_SIGNALS::OK;
}

RETURNED_SIGNALS List::erase_by_logical_index (size_t logical_index) {

    ASSERT_OK
    
    size_t phys_ind = 0;
    if (get_physical_index_by_logical_index (logical_index, &phys_ind) != RETURNED_SIGNALS::OK) {
        return RETURNED_SIGNALS::WRONG_INDEX;
    }

    ASSERT_OK

    return erase(phys_ind);
}

RETURNED_SIGNALS List::pop_back() {
    ASSERT_OK
    return erase(tail);
}

RETURNED_SIGNALS List::pop_front() {
    ASSERT_OK
    return erase(head);
}

//----------------------------------------------------------------------sort----------------------------------------------------------------------

RETURNED_SIGNALS List::sort(){

    ASSERT_OK

    Node* buf = (Node*)calloc(capacity, sizeof(Node));
    
    if (buf == nullptr) {
        return RETURNED_SIGNALS::SORT_FAILURE;
    }

    buf[0] = {elements[0].data, 1, size};
    
    int curr = 1;
    for(int i = head; i != 0; i = elements[i].next, curr++){
        //                       data,        next,         prev 
        buf[curr] = {elements[i].data,  (curr + 1),   (curr - 1)};
    }

    is_sorted = true;
    for(int i = curr; i < capacity; i++){
        //          data,   next, prev 
        buf[i] = {POISON,  i + 1,   -1};
    }
    
    free(elements);

    elements = buf;

    head = elements[0].next;
    tail = elements[0].prev;

    elements[head].prev = 0;
    elements[tail].next = 0;

    first_free = size + 1; 

   /* for (int curr = head, index = 1; index < size && curr != 0; index++) {
        int next_curr = elements[curr].next;
        printf("index = %d, curr = %d\n", index, curr);
        swap(&elements[index], &elements[curr], index);
        curr = next_curr;
    } */

    ASSERT_OK

    return RETURNED_SIGNALS::OK;
}

//-----------------------------------------------------------physical and logical index-----------------------------------------------------------

RETURNED_SIGNALS List::get_physical_index_by_logical_index(size_t logical_ind, size_t* physical_ind) {

    ASSERT_OK
    
    if (logical_ind > size) {
        return RETURNED_SIGNALS::WRONG_INDEX;
    }

    if (is_sorted) {
        *physical_ind = logical_ind;
    }
    
    else {
        size_t curr = 0;
        if (logical_ind < size/2) {
            for (int i = 1; i <= logical_ind; i++) {
                curr = elements[curr].next;
            }
        }
        else {
            for (int i = 0; i <= size - logical_ind; i++) {
                curr = elements[curr].prev;
            }
        }
        *physical_ind = curr;
    }

    ASSERT_OK

    return RETURNED_SIGNALS::OK;
} 

RETURNED_SIGNALS List::data_by_physical_index(size_t index, elem_t* val) {

    ASSERT_OK
    
    if (elements[index].prev == -1) {
        return RETURNED_SIGNALS::WRONG_INDEX;
    }

    *val = elements[index].data;

    ASSERT_OK

    return RETURNED_SIGNALS::OK;
}

RETURNED_SIGNALS List::data_by_logical_index(size_t index, elem_t* val){

    ASSERT_OK

    size_t phys_index = 0;

    if (get_physical_index_by_logical_index(index, &phys_index) != RETURNED_SIGNALS::OK) {
        return RETURNED_SIGNALS::INDEX_FUNC_FAILURE;
    }

    if (elements[index].prev == -1) {
        return RETURNED_SIGNALS::WRONG_INDEX;
    }
    data_by_physical_index(phys_index, val);

    ASSERT_OK
}

//-----------------------------------------------------------------dump functions-----------------------------------------------------------------

void List::dump_list(int test_num, int pass) {

    #define Xprintf_img(format, ...)  fprintf(dump_file, format, __VA_ARGS__);
    #define Xprint_img(str)           fprintf(dump_file, "%s", str);

    FILE* dump_file = fopen("dump.gz", "w");
    
    Xprint_img("digraph G {\n    rankdir = \"LR\";\n");
    
    Xelem(0, ORDINARY);


    for (int i = 0; i < capacity; i++) {

        if (elements[i].prev == -1) { 
            Xelem(i, ORDINARY)
            Xline_next(i);
            if ((i - 1 > 0) && (elements[i-1].prev == -1)) {Xaux_line_prev(i);}
        }
        else if (i == head) {
            Xelem(i, HEAD)
            if (i == 0) {
                Xprint_img("elem_0:next -> elem_1:ind ;\n"); Xprint_img("elem_1:prev -> elem_0:ind [color=blue];\n");
            }
            else {
                Xline_next(i);
                Xline_prev(i);
            }
        }
        else if (i == tail) {
            Xelem(i,TAIL)
            if (i == 0) {
                Xprint_img("elem_0:next -> elem_1:ind ;\n"); Xprint_img("elem_1:prev -> elem_0:ind [color=red];\n");
            }
            else Xline_prev(i);
        }
        else if (i == 0) {
            Xelem(i, ORDINARY)  
            // Xaux_line_next(i); 
            Xprintf_img("elem_0:next -> elem_%d:ind ;\n", elements[0].next);
        }
        else {   
            Xelem(i, ORDINARY)
            Xline_next(i);
            if (is_sorted == 1) {
                Xaux_line_prev(i);
            }
            else Xline_prev(i);
        }
    }

    Xprint_img("}\n");
    fclose(dump_file);  

    char* buf = (char*) calloc(100, sizeof(char));
    snprintf(buf, 100, "dot -q -Tpng dump.gz -o imgs/test_%d_%d.png", test_num, pass);
    system(buf);

    free(buf);
    
    dump_table(test_num, pass);
}

void List::dump_table(int test_num, int pass) {

    FILE* table_file = fopen("table.gz", "w");

    #define Xprintf_img(format, ...)  fprintf(table_file, format, __VA_ARGS__);

    Xprintf_img("digraph Table{\n"
                "node [ shape=none fontname=Helvetica ]\n"
                "n3 [ label = <\n"
                "<table>\n"
                "    <tr><td>List state</td><td> %s </td></tr>\n"
                "    <tr><td>Size</td><td> %d </td></tr>\n"
                "    <tr><td>Capacity</td><td> %d </td></tr>\n"
                "    <tr><td>Head</td><td> %d </td></tr>\n"
                "    <tr><td>Tail</td><td> %d </td></tr>\n"
                "    <tr><td>First free element  </td><td> %d </td></tr>\n"
                "</table>\n"
                "> ]\n"
                "}\n", END_SIGNALS[status], size, capacity, head, tail, first_free);
    fclose(table_file);


    char* buf = (char*) calloc(100, sizeof(char));
    snprintf(buf, 100, "dot -q -Tpng table.gz -o imgs/table_%d_%d.png", test_num, pass);
    system(buf);
    free(buf);     
}

//-----------------------------------------------------------------verification-----------------------------------------------------------------

void print_log_wrappaer (void* someObj) {
    List* list_tmp = (List*) someObj;
    list_tmp->print_error_message_to_log_file(list_tmp->verify());
}

int List::print_error_message_to_log_file(int error) {
    
    int error_name_index = 0;

    fprintf(log_file, "\n\nList statues: " );
                      

    if (error & 1 == 1) {
        fprintf(log_file, "%s\n", ERROR_NAMES[error_name_index]);
        return 0;
    }
    else {
        error_name_index++;
    }
    
    compare_error_code_with_mask(error, ERRORS::ZERO_ELEMENT_ERROR, &error_name_index);
    compare_error_code_with_mask(error, ERRORS::NEXT_ERROR,         &error_name_index);
    compare_error_code_with_mask(error, ERRORS::PREV_ERROR,         &error_name_index);
    compare_error_code_with_mask(error, ERRORS::SIZE_ERROR,         &error_name_index);    
}

void List::compare_error_code_with_mask(int error, ERRORS mask, int* error_name_index) {
    if ((error & (int)mask)) {
        fprintf(log_file, ERROR_NAMES[*error_name_index]);
    }
    *error_name_index++;
}    

int List::verify() {
    return (int) verify_zero_element() | (int) verify_head_to_tail() | (int) verify_tail_to_head() | (int) verify_size();
}

ERRORS List::verify_zero_element() {
    if (/*elements[0].next != head || elements[0].prev != tail || elements[0].data != POISON*/0) {
        return ERRORS::ZERO_ELEMENT_ERROR;
    }
    return ERRORS::OK;
}

ERRORS List::verify_head_to_tail() {
    for (int index = head, count = 1; count <= size; count++, index = elements[index].next) {
        if (index <= 0 || index >= capacity || elements[index].data == POISON || elements[index].prev == -1 ) {
            return ERRORS::NEXT_ERROR;
        }
    }
    return ERRORS::OK;
}

ERRORS List::verify_tail_to_head() {
    for(int index = tail, count = 1; count <= size; count++, index = elements[index].prev){
        if(index <= 0 || index > capacity){
            return ERRORS::PREV_ERROR;
        }
    }
    return ERRORS::OK;
}

ERRORS List::verify_size() {
    if (first_free <= 0 || size >= capacity || head > capacity || tail > capacity || first_free > capacity) {
        return ERRORS::SIZE_ERROR;
    }
    ERRORS::OK;
}
