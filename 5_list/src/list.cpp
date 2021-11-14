#include "list.h"

char* test_names[] = {

    "Push back",               // 1
    "Push back",               // 2
    "Push back",               // 3
    "Push back",               // 4
    "Push back",               // 5
    "Push back",               // 6
  
    "Push front",              // 7
    "Push front",              // 8
    "Push front",              // 9
    "Push front",              // 10
    "Push front",              // 11
    "Push front",              // 12
    "Push front",              // 13
    "Push front",              // 14
    "Push front",              // 15
    "Push front",              // 16
    "Push front",              // 17
  
    "Erase",                   // 18
    "Erase by logical index",  // 19
 
    "Insert",                  // 20
    "Insert by logical index", // 21
    
    "Pop back",                // 22
    "Pop front",               // 23

    "Sort",                    // 24
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

    for (int i = 0; i < capacity; i++) {
        //               data,  next, prev 
        elements[i] = {POISON,  i+ 1,   -1};
    }

    elements[0] = {POISON, 0, 0};
    return RETURNED_SIGNALS::OK;
}

RETURNED_SIGNALS List::Dtor() {

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
   return elements[head].data; 
}

elem_t List::back() {
   return elements[tail].data; 
}

RETURNED_SIGNALS  List::increase_capacity() {

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
    return RETURNED_SIGNALS::OK;
}

RETURNED_SIGNALS  List::decrease_capacity() {

    if (is_sorted == false) {
        printf("Unable to decrease. Sort the list first\n");
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

    return RETURNED_SIGNALS::OK;
}

//-------------------------------------------------------------list insert functions--------------------------------------------------------------

RETURNED_SIGNALS List::insert(size_t index, const elem_t* value) {

    if (((tail == capacity - 1) || (size >= capacity - 1)) && first_free == capacity) {
        if (increase_capacity() != RETURNED_SIGNALS::OK) {
            return RETURNED_SIGNALS::INCREASE_FAILURE;
        }
    }

    if((index > capacity) || (elements[index].prev == -1)){
        return RETURNED_SIGNALS::WRONG_INDEX;
    }

    if (index != elements[first_free].prev) is_sorted = 0;
    
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

    return RETURNED_SIGNALS::OK;
}

RETURNED_SIGNALS List::insert_by_logical_index (size_t logical_index, elem_t* val) {

    size_t phys_index = 0;
    get_physical_index_by_logical_index(logical_index, &phys_index);
    insert(phys_index, val);
}

RETURNED_SIGNALS List::push_back(const elem_t* value) {
    return insert(tail, value);
}

RETURNED_SIGNALS List::push_front(const elem_t* value) {
    return insert(0, value);
}

//----------------------------------------------------------list erese and pop functions----------------------------------------------------------

RETURNED_SIGNALS List::erase(size_t index) {

    if((index == 0) || (index >= capacity) || (elements[index].prev == -1)){
        return RETURNED_SIGNALS::WRONG_INDEX;
    }

    if((index != tail) || (index != head)){
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
    
    // *value = elements[index].data;
    // elements[index].next = first_free;
    elements[index].prev =         -1;
    elements[index].data =     POISON;
    // first_free = index;
    
    head = elements[0].next;
    tail = elements[0].prev;
    size--;

    if((size < (capacity / 2)) && (is_sorted == true)){
        printf("DECREASE head = %d, tail = %d, size = %d, capacity = %d\n", head, tail, size, capacity / 2);
        if (decrease_capacity() != RETURNED_SIGNALS::OK) {
            return RETURNED_SIGNALS::DECREASE_FAILURE;
        }
    }
    return RETURNED_SIGNALS::OK;
}

RETURNED_SIGNALS List::erase_by_by_logical_index (size_t logical_index) {
    
    size_t phys_ind = 0;
    if (get_physical_index_by_logical_index (logical_index, &phys_ind) != RETURNED_SIGNALS::OK) {
        return RETURNED_SIGNALS::WRONG_INDEX;
    }
    return erase(phys_ind);
}

RETURNED_SIGNALS List::pop_back() {
    return erase(tail);
}

RETURNED_SIGNALS List::pop_front() {
    return erase(head);
}

//----------------------------------------------------------------------sort----------------------------------------------------------------------

RETURNED_SIGNALS List::sort(){

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

    return RETURNED_SIGNALS::OK;
}

//-----------------------------------------------------------physical and logical index-----------------------------------------------------------

RETURNED_SIGNALS List::get_physical_index_by_logical_index(size_t logical_ind, size_t* physical_ind) {
    
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

    return RETURNED_SIGNALS::OK;
} 

RETURNED_SIGNALS List::data_by_physical_index(size_t index, elem_t* val){
    
    if (elements[index].prev == -1) {
        return RETURNED_SIGNALS::WRONG_INDEX;
    }

    *val = elements[index].data;
    return RETURNED_SIGNALS::OK;
}

RETURNED_SIGNALS List::data_by_logical_index(size_t index, elem_t* val){

    size_t phys_index = 0;

    if (get_physical_index_by_logical_index(index, &phys_index) != RETURNED_SIGNALS::OK) {
        return RETURNED_SIGNALS::INDEX_FUNC_FAILURE;
    }

    if (elements[index].prev == -1) {
        return RETURNED_SIGNALS::WRONG_INDEX;
    }
    data_by_physical_index(phys_index, val);
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
    snprintf(buf, 100, "dot -Tpng dump.gz -o imgs/test_%d_%d.png", test_num, pass);
    system(buf);

    free(buf);
    
    dump_table(test_num, pass);
}

void List::do_html(int num_of_tests) {

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
        
        Xprintf_html("    <h1 style=\"color:black;background-color:#F9E79F;text-align:center;\">Test %d. %s</h1>\n"
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
                    
                    i, test_names[i-1], i, i,/* END_SIGNALS[status], size(), capacity, head, tail, first_free,*/ i, i);
    }

    Xprint_html("</body>\n"
                "</html>\n");

    fclose(html_file);

    system("firefox dump.html");
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
    snprintf(buf, 100, "dot -Tpng table.gz -o imgs/table_%d_%d.png", test_num, pass);
    system(buf);
    free(buf);     
}