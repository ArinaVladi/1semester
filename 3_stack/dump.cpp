#include "dump.h"

void method_error (int status) {

    char* message = (char*) calloc (1, sizeof (char));
    for (int error_mask = 0, error_pow = 0; error_pow <= MAX_ERROR_POW; error_mask *= 2, error_pow++) {
        if ( (status & error_mask) == error_mask) {
            message = (char*)realloc (message, (strlen (message) + strlen (METHOD_ERROR_NAMES[error_pow]) + 1)*sizeof (char));
            strcat (message, METHOD_ERROR_NAMES[error_pow]);
        }
    }
    print_method_error (message);
}

void print_method_error (char* message) {

    FILE* log_f = fopen ("log_file.txt", "a");
    fprintf (log_f,  "--------------------------------------------------------------------------------\n"
                    "ERROR: %s\n", message);

    fclose (log_f);
    assert (0);
}

//----------------------------------------struct Dump methods----------------------------------------

void dump_t::clear_file () {
    log_file = fopen (file_name, "w");
    assert (file_name);
    fclose (log_file);
}

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

void dump_t::print_dump_impl (void* obj_address, void (*print_dump) (void* someStack), char* error_message, int error) {

    if (error_message[0] == '0') {
        error_message = "was not provided\n";
    }

    log_file = fopen (file_name, "a");
    assert (file_name);

    fprintf (log_file,  "--------------------------------------------------------------------------------\n"
                    "%s %s\n"
                    "ERROR: %s\n"                        // optional
                    "DUMP from:\n"
                    "    file:           %s\n"
                    "    function:       %s\n"
                    "    line:           %d\n"
                    "    name:           %s\n"            // optional
                    "    object address: %p %s\n"
                    "    object type:    %s\n"            // optional
                    "    data type:      %s\n"            // optional
                    "    data elem size: %d\n",           // optional

                    __DATE__, __TIME__,
                    error_message,                        // optional
                    file,
                    function,
                    line,
                    name,                                 // optional
                    address, IS_OK (error == NULL_STACK_PTR),
                    obj_type,                             // optional
                    elem_type,                            // optional
                    elem_size                             // optional
                    );
    fclose (log_file);

    print_dump (obj_address);
}
