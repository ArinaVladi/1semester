#include "list.h"

char* test_names[] = {
    "Push back",
    "Push front",
};

char* END_SIGNALS[] = {
    "OK",
    "ERROR"
};

enum END_SIGNALS_NAMES {
    OK    = 0,
    ERROR = 1
};

void List::Ctor() {

    capacity = CAPACITY;
    elements = (Node*) calloc(capacity, sizeof(Node));
    head       =    0;
    tail       =    0;
    first_free =    1;
    is_sorted  = true;
    status     =    0;

    for (int i = 0; i < capacity; i++) {
        elements[i].data = POISON;
        elements[i].next = i + 1;
        elements[i].prev = -1; 
    }

    elements[0] = {POISON, 0, 0};
}

//--------------------------------------------------------------list size functions---------------------------------------------------------------


elem_t* List::front() {
   return &(elements[head].data); 
}

elem_t* List::back() {
   return &(elements[tail-1].data); 
}

bool List::empty() {
    return (head - tail < 0);
}

unsigned int List::size() {
    return (unsigned int)(tail - head + 1);
}

void List::increase_capacity() {

    int increased_capacity = 2 * capacity;
    Node* increased_elements = (Node*) calloc(increased_capacity, sizeof(Node));
    assert(increased_elements != nullptr);

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
}

//-------------------------------------------------------------list insert functions--------------------------------------------------------------
static int num = 12;
void List::insert(int index, const elem_t* value) {

    if (((tail == capacity - 1) || (size() >= capacity - 1)) && first_free == capacity) {
        increase_capacity();
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
    num++;
}

void List::push_back(const elem_t* value) {

    assert(tail+1 <= capacity);

    insert(tail, value);
}

void List::push_front(const elem_t* value) {

    assert(tail+1 <= capacity);

    insert(head, value);
}



//-----------------------------------------------------------------dump functions-----------------------------------------------------------------

void List::dump(int test_num, int pass) {

    FILE* dump_file = fopen("tmp.gz", "w");
    Xprint_img("digraph G {\n    rankdir = \"LR\";\n");
    
    Xelem(0, ORDINARY);


    for (int i = 0; i < capacity; i++) {

        if (elements[i].prev == -1) { 
            Xelem(i, ORDINARY)
            Xaux_line_next(i);
            if ((i - 1 > 0) && (elements[i-1].prev == -1)) Xaux_line_prev(i);
        }
        else if (i == head) {
            Xelem(i, HEAD)
            if (i == 0) {Xprint_img("elem_0:next -> elem_1:ind ;\n"); Xprint_img("elem_1:prev -> elem_0:ind [color=blue];\n"); }
            else {Xline_next(i);
                 Xaux_line_prev(i);}
        }
        else if (i == tail) {
            Xelem(i,TAIL)
            if (i == 0) {Xprint_img("elem_0:next -> elem_1:ind ;\n"); Xprint_img("elem_1:prev -> elem_0:ind [color=red];\n"); }
            else Xaux_line_prev(i);
        }
        else if (i == 0) {
            Xelem(i, ORDINARY)  
            Xaux_line_next(i); 
        }
        else {   
            Xelem(i, ORDINARY)
            Xline_next(i);
            if (is_sorted == 1) { Xaux_line_prev(i) }
            else Xline_prev(i);
        }
        
    }

    Xprint_img("}\n");
    fclose(dump_file);  

    char* buf = (char*) calloc(100, sizeof(char));
    snprintf(buf, 100, "dot -Tpng tmp.gz -o imgs/test_%d_%d.png", test_num, pass);
    system(buf);
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
                    "    <table style=\"width:20%\">\n"
                    "        <tr>\n"
                    "            <td>List state</td>\n"
                    "            <td>%s</td>\n"
                    "        </tr>\n"
                    "        <tr>\n"
                    "            <td>Size</td>\n"
                    "            <td>%d</td>\n"
                    "        </tr>\n"
                    "        <tr>\n"
                    "            <td>Capacity</td>\n"
                    "            <td>%d</td>\n"
                    "        </tr>\n"
                    "        <tr>\n"
                    "            <td>Head</td>\n"
                    "            <td>%d</td>\n"
                    "        </tr>\n"
                    "        <tr> \n"
                    "            <td>Tail</td>\n"
                    "            <td>%d</td>\n"
                    "        </tr>\n"
                    "        <tr>\n"
                    "            <td>First free element</td>\n"
                    "            <td>%d</td>\n"
                    "        </tr>\n"
                    "    </table>\n"
                    "    <p> </p>\n"
                    "    <p style=\"background-color:#FCF9E7;text-align:center;\">Before</p>\n"
                    "    <img src=\"imgs/test_%d_1.png\" alt=\"before\">\n"
                    "    <p> </p>\n"
                    "    <p style=\"background-color:#FCF9E7;text-align:center;\">After</p>\n"
                    "    <img src=\"imgs/test_%d_2.png\" alt=\"after\">\n"
                    "    <p> </p>\n"
                    "    <p> </p>\n"
                    "    <p> </p>\n",
                    
                    i, test_names[i-1], END_SIGNALS[status], size(), capacity, head, tail, first_free, i, i);
    }

    Xprint_html("</body>\n"
                "</html>\n");

    fclose(html_file);

    system("firefox dump.html");
}