#include "log.h"
#include "constants.h"

void dump_t::initialize (void*             obj_address,
                         char*            current_file,
                         const char*      current_func,
                         int              current_line,
                         const char*          obj_name,
                         int         current_elem_size,
                         char*        current_obj_type,
                         char*       current_elem_type) {

    name      = (obj_name[0] == '0')? ELEMENT_NOT_SPECIFIED : obj_name;
    address   =         obj_address;
    file      =        current_file;
    function  = (char*)current_func;
    line      =        current_line;
    elem_size =   current_elem_size;
    obj_type  =    current_obj_type;
    elem_type =   current_elem_type;
}

 

void dump_t::print_dump_impl (void* obj_address, void (*print_dump) (void* someObj), FILE* log_file) {
    
    fprintf (log_file,  "--------------------------------------------------------------------------------\n"
                        "%s\n"
                        "DUMP from:\n"
                        "    file:           %s\n"
                        "    function:       %s\n"
                        "    line:           %d\n"
                        "    name:           %s\n"            // optional
                        "    object address: %p\n" 
                        "    object type:    %s\n"            // optional
                        "    data type:      %s\n"            // optional
                        "    data elem size: %d\n",           // optional*/
    
                        __DATE__,
                        file,
                        function,
                        line,
                        name,                                 // optional
                        address,  
                        obj_type,                             // optional
                        elem_type,                            // optional
                        elem_size                             // optional */
            );
    print_dump (obj_address);
}