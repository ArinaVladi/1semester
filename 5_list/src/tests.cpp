#include "list.h"

int main() {

    List list{};

    list.Ctor();
    list.dump(1, 1);
    for (int i = 12; i <= 21; i++) {
        elem_t val = i * 1.1;
        list.push_back(&val);
    }
    list.dump(1, 2);

    //list.Ctor();
    list.dump(2, 1);
    for (int i = 12; i <= 12; i++) {
        elem_t val = i * -1.1;
        list.push_front(&val);
    }
    list.dump(2, 2);
      
    list.do_html(2);
}