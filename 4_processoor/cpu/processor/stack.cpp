#include "stack.h"
#if defined VERIFIER
void my_fprintf (FILE* log_f, int num, int elem) {
    fprintf (log_f, "    %7s | [%4d] | %13d | %s\n",  IS_INIT (elem), num, elem,IS_POISON(elem));
}
void my_fprintf (FILE* log_f, int num, double elem) {
    fprintf (log_f, "    %7s | [%4d] | %13lg | %s\n", IS_INIT (elem), num, elem,IS_POISON(elem));
}
void my_fprintf (FILE* log_f, int num, float elem) {
    fprintf (log_f, "    %7s | [%4d] | %13f | %s\n",  IS_INIT (elem), num, elem,IS_POISON(elem));
}
/*void my_fprintf (FILE* log_f, int num, char* elem) {
    fprintf (log_f, "    %7s | [%4d] | %13d | %s\n", IS_INIT (elem), num, elem,IS_POISON(elem));
}*/
#endif



//----------------------------------------get const canary----------------------------------------

static canary_t get_canary (){    //! inside methodse
    srand (time (nullptr));
    return rand ();
}
IF_CANARY_PROTECTION (const canary_t CANARY_VAL = get_canary ());


//---------------------------------------struct stack_t methods--------------------------------------

int stack_t::Ctor (int size_of_elem) {

    Print_Debug ("Creation of stack starting...");

    if (size_of_elem <= 0) {
        return CTOR_ERROR | NEGATIVE_ELEM_SIZE;
    }

    capacity  = INIT_CAPACITY;
    size      =             0;
    elem_size =  size_of_elem;

    IF_CANARY_PROTECTION (canary_r = CANARY_VAL);
    IF_CANARY_PROTECTION (canary_l = CANARY_VAL);

    char* tmp_data = (char*) calloc (1,  elem_size * capacity IF_CANARY_PROTECTION ( + 2 * sizeof (canary_t)));
    if (tmp_data == nullptr) {
        return CTOR_ERROR | MEMORY_ALLOCATION_ERROR;
    }
    #ifdef CANARY_PROTECTION
        memcpy (tmp_data, &CANARY_VAL, sizeof (canary_t));
        memcpy (tmp_data + elem_size * capacity + sizeof (canary_t), &CANARY_VAL, sizeof (canary_t));
    #endif //CANARY_PROTECTION

    data = (void*) (tmp_data IF_CANARY_PROTECTION (+ sizeof (canary_t)));   //! сдвинуть на (16 байт - sizeof(canary_t))про 16
    for (int i = 0; i<capacity; i++) {
        void* tmp_data = (char*)data + elem_size * i;
        memcpy (tmp_data, &INT_POISON, elem_size);
    }

    IF_HASH_PROTECTION (get_hash_wrapper ());

    PD_Value ("Struct hash = %d\n", struct_hash);
    Print_Debug ("Before stackOK");

    ASSERT_OK

    Print_Debug ("Stack created");

    return STACK_OK;
}

int stack_t::increase () {

    Print_Debug ("Stack increase starting...")

    ASSERT_OK

    PD_Value ("Stack capacity = %d, stack size = %d. Stack must b increased.\n", capacity, size);

    char* tmp_data = (char*)calloc (1, elem_size * capacity * MULTIPLIER IF_CANARY_PROTECTION ( + sizeof (canary_t) * 2));

    if (tmp_data == nullptr) {
        return STACK_INCREASE_ERROR;
    }

    #if defined CANARY_PROTECTION
        memcpy (tmp_data, &CANARY_VAL, sizeof (canary_t));
        memcpy (tmp_data + sizeof (canary_t) + elem_size* (capacity * MULTIPLIER) , &CANARY_VAL, sizeof (canary_t));
    #endif //CANARY_PROTECTION

    void* data_copy = (char*) (tmp_data IF_CANARY_PROTECTION ( + sizeof (canary_t)));

    for (int i = 0; i < size; i++){
        void* tmp_data;
        tmp_data = (char*)data_copy + elem_size * i;
        memcpy (tmp_data, (char*)data + elem_size * i, elem_size);
    }

    free ( (char*)data IF_CANARY_PROTECTION (- sizeof (canary_t)));

    capacity *= MULTIPLIER;
    data      =  data_copy;

    for (int i = size; i < capacity; i++) {
        void* tmp_data;
        tmp_data = (char*)data + elem_size * i;
        memcpy (tmp_data, &INT_POISON, elem_size);
    }

    IF_HASH_PROTECTION (get_hash_wrapper ());
    ASSERT_OK

    PD_Value ("New stack capacity = %d, new stack size = %d\n", capacity, size);

    Print_Debug ("Stack increase finished.")
    return STACK_OK;
}

int stack_t::push (const void* value) {

    ASSERT_OK

    if (size >= capacity) {
        if (increase () == STACK_INCREASE_ERROR) {
            return PUSH_ERROR;
        }
    }

    void* tmp_data = (char*)data + elem_size*size;
    memcpy (tmp_data, value, elem_size);
    size++;

    IF_HASH_PROTECTION (get_hash_wrapper ());

    ASSERT_OK
    return STACK_OK;
}


int stack_t::pop () {

    ASSERT_OK

    if (size <= 0) {
        return POP_ERROR | STACK_IS_EMPTY;
    }

    void* tmp_data = (char*)data + elem_size* (size-1);
    memcpy (tmp_data, &INT_POISON, elem_size);
    size--;

    PD_Value ("Difference between capacity and size = %d\n", capacity - size);
    if (capacity - size >= capacity/2-1 && size > INIT_CAPACITY) {
        IF_HASH_PROTECTION (get_hash_wrapper ());
        if (shrink_stack () == STACK_SHRINK_ERROR){
            return POP_ERROR | STACK_SHRINK_ERROR;
        }
    }

    IF_HASH_PROTECTION (get_hash_wrapper ());

    ASSERT_OK
    return STACK_OK;
}

void stack_t::top (void* target) {

    ASSERT_OK

    void* tmp_data = (char*)data + elem_size* (size-1);
    memcpy (target, tmp_data, elem_size);
    IF_HASH_PROTECTION (get_hash_wrapper ());

    ASSERT_OK
}

int stack_t::shrink_stack () {

    ASSERT_OK

    PD_Value ("Stack capacity = %d, stack size = %d.\nThe difference is %d. Stack must be decreased.", capacity, size, capacity-size);

    char* tmp_data = (char*)calloc (1, elem_size * size IF_CANARY_PROTECTION ( + 2 * sizeof (canary_t)));
    if (tmp_data == nullptr) {
        return STACK_SHRINK_ERROR;
    }

    #if defined CANARY_PROTECTION
        memcpy (tmp_data, &CANARY_VAL, sizeof (canary_t));
        memcpy (tmp_data + elem_size * size + sizeof (canary_t), &CANARY_VAL, sizeof (canary_t));
    #endif //CANARY_PROTECTION

    void* data_copy = (char*) (tmp_data IF_CANARY_PROTECTION (+ sizeof (canary_t)));

    for (int i = 0; i < size; i++){
        void* tmp_data;
        tmp_data = (char*)data_copy + elem_size * i;
        memcpy (tmp_data, (char*)data + elem_size * i, elem_size);
    }

    free ( (char*)data IF_CANARY_PROTECTION ( - sizeof (canary_t)));

    capacity =      size;
    data     = data_copy;

    IF_HASH_PROTECTION (get_hash_wrapper ());

    ASSERT_OK

    return STACK_OK;
}

void stack_t::Dtor () {

    ASSERT_OK

    free ( (char*)data IF_CANARY_PROTECTION ( - sizeof (canary_t)));
}

#if defined HASH_PROTECTION
uint32_t stack_t::get_hash (void *some_data, int size) {

    char* message = (char*) some_data;
    uint32_t byte, crc, mask;

    crc = 0x7FFFFFFF;
    for (int n = 0; n < size; n++) {
        byte = message[n];
        crc = crc ^ byte;
        for (int j = 7; j >= 0; j--) {
            mask = - (crc & 1);
            crc = (crc >> 1) ^ (0xDEADBEEF & mask);
        }
    }

    return ~crc;
}

void stack_t::get_hash_wrapper (){

    #if defined HASH_PROTECTION
        struct_hash =  get_hash (this, sizeof (stack_t) - 2*sizeof (struct_hash) IF_CANARY_PROTECTION (-sizeof (canary_t)));
        data_hash   =  get_hash (data, (int) (size * elem_size));
    #endif //HASH_PROTECTION
}
#endif //HASH_PROTECTION

#if defined VERIFIER
int stack_t::stackOK (){

    Print_Debug ("stackOK starting")

    PD_Value ("size = %d\n", size);

    int error_num = 0;

    if (data == nullptr) {
        error_num |= NULL_STACK_DATA_PTR;
        PD_Value ("## Stack data ptr is null. error code = %d\n", error_num)
    }
    if (capacity < 0) {
        error_num |= NEGATIVE_CAPACITY;
        PD_Value ("## Capacity is negative. error code = %d\n", error_num)
    }
    if (size < 0) {
        error_num |= NEGATIVE_SIZE;
        PD_Value ("## size is negative. error code = %d\n", error_num)
    }
    if ( (capacity < size) && size > 0){
        PD_Value ("!!!!!!!!!!!!size = %d\n", size)
        error_num |= OVERFLOW;
        PD_Value ("## size > capacity. error code = %d\n", error_num)
    }

    #if defined HASH_PROTECTION
    if (struct_hash !=  get_hash (this, sizeof (stack_t) - 2*sizeof (struct_hash)  IF_CANARY_PROTECTION (-sizeof (canary_t)))) {
            error_num |= STRUCT_HASH_DONT_MATCH;
        }

        if (data_hash !=  get_hash (data, (int) (size * elem_size))){
            error_num |= DATA_HASH_DONT_MATCH;
        }
    #endif //HASH_PROTECTION

    #ifdef CANARY_PROTECTION

        Print_Debug ("Canary protection")
        if (canary_l != CANARY_VAL) {
            error_num |= CANARY_L_DESTROYED;
            PD_Value ("## Left canary is destroyed. error code = %d\n", error_num)
        }
        if (canary_r != CANARY_VAL) {
            error_num |= CANARY_R_DESTROYED;
            PD_Value ("## Left canary is destroyed. error code = %d\n", error_num)
        }
    #endif //CANARY_PROTECTION

    Print_Debug ("stackOK before return")

    PD_Value ("\n\n\nerr_no = %d\n\n\n", error_num);

    Print_Debug ("stackOK finished")

    return error_num;
}

void stack_t::print_stack_dump (int error) {

    FILE* log_f = fopen (Dump::file_name, "a");
    assert (log_f);

    fprintf (log_f,  "Mode:\n"
                    "%s"
                    "\nStack data:\n\n"
                    "\tsize     = %10d %s\n"
                    "\tcapacity = %10d %s\n\n",

                    IS_MODE,
                    size,
                    IS_OK ( (error == NEGATIVE_SIZE)     || (error == OVERFLOW)),
                    capacity,
                    IS_OK ( (error == NEGATIVE_CAPACITY) || (error == OVERFLOW))
                    );

    Print_Debug (" ")
    IF_CANARY_PROTECTION (fprintf (log_f, "\tleft  canary = %10d %s\n  ", canary_l, IS_OK (error == CANARY_L_DESTROYED)));
    IF_CANARY_PROTECTION (fprintf (log_f, "\tright canary = %10d %s\n\n", canary_r, IS_OK (error == CANARY_R_DESTROYED)));

    IF_HASH_PROTECTION (fprintf (log_f, "\tdata   hash status = %s\n  ", IS_OK (error == DATA_HASH_DONT_MATCH)));
    IF_HASH_PROTECTION (fprintf (log_f, "\tstruct hash status = %s\n\n", IS_OK (error == STRUCT_HASH_DONT_MATCH)));

    fprintf (log_f, "    %s | %s | %s | %s\n", "is init", "number", "    value    ", "status");
    Print_Debug (" ")
    char* sth = (char*) calloc (1, elem_size); // to make buffer independent of type
    for (int i  = 0; i < capacity; i++) {
        void* tmp_data = (char*) data + elem_size*i;
        memcpy (sth, tmp_data, elem_size);
        /*fprintf (log_f, "    %7s | [%4d] | %13d | %s\n", IS_INIT (*(int*)sth), i, sth,IS_POISON (*(int*)sth));*/
        my_fprintf (log_f, i, *(int*)sth);
    }
    free(sth);

    fclose (log_f);
}

//-------------------------------function to pass to struct Dump method print_dump_impl------------------------------

void print_dump_wrapper_void_to_stack (void* someStack) {
    stack_t* stack_tmp = (stack_t*) someStack;
    stack_tmp->print_stack_dump (stack_tmp->stackOK ());
}

//--------------------------function to get error name as a char** from enum of error names--------------------------

char* get_error_name (int error_code) {

    char* error_name = (char*)calloc (5, sizeof (char));
    for (int error_mask = 1, error_pow = 0; error_pow <= MAX_ERROR_POW; error_mask *= 2, error_pow++) {
        if ( (error_code & error_mask) == error_mask) {
            error_name = (char*)realloc (error_name, (strlen (error_name)+strlen (ERROR_NAMES[error_pow])+1)*sizeof (char));
            strcat (error_name, ERROR_NAMES[error_pow]);
        }
    }
    return error_name;
}
#endif //VERIFIER
