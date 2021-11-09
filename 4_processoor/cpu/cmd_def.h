#define CUR_COMMAND program[IP++] & PROGRAM_CODE_MASK
#define CUR_ARG(size) get_cur_arg(size)

//#define DEBUG_CPU

#if defined DEBUG_CPU
#define IF_CPU_DUMP(code) code
#else
#define IF_CPU_DUMP(code)
#endif

//-----------------------------------------------------PUSH and POP-----------------------------------------------------

#define STACK_PUSH(x)                                                                                                  \
        theStack.push(x);

#define STACK_POP(x)                                                                                                   \
        theStack.top(x);                                                                                               \
        theStack.pop();

#define IS(x, mask) ((x & mask) == mask)

#define GET_ARG_TYPE(num)                                                                                              \
    (IS(num, RAM_REG_CONST))?             RAM_REG_CONST_ARG :                                                          \
        ((IS(num, RAM_REG))?              RAM_REG_ARG       :                                                          \
            ((IS(num, RAM_CONST))?        RAM_CONST_ARG     :                                                          \
                ((IS(num, REG))?          REG_ARG           :                                                          \
                    ((IS(num, CONST))?    CONST_ARG         :                                                          \
                                          WRONG_ARG))))

#define GET_REG(x) &reg_ ## x

#define CUR_REG_NUM *(program + IP)

#define REG_EQUIV(x) (1>>(x^CUR_REG_NUM))

#define REG_NUM get_cur_reg()

#define GET_REG_NUM                                                                                                    \
    ((REG_EQUIV(1))?                       GET_REG(1) :                                                                \
        ((REG_EQUIV(2))?                   GET_REG(2) :                                                                \
            ((REG_EQUIV(3))?               GET_REG(3) :                                                                \
                ((REG_EQUIV(4))?           GET_REG(4) :                                                                \
                    ((REG_EQUIV(5))?       GET_REG(5) :                                                                \
                                           NULL)))))

// always returns address
#define GET_ARG(arg_type)                                                                                              \
    (arg_type == RAM_REG_CONST_ARG)?          &ram[ (int)(*REG_NUM + *CUR_ARG(sizeof(int))) ] :                        \
        ((arg_type == RAM_REG_ARG)?           &ram[ (int)(*REG_NUM) ]                         :                        \
            ((arg_type == RAM_CONST_ARG)?     &ram[ (int)(*CUR_ARG(sizeof(int))) ]            :                        \
                ((arg_type == REG_ARG)?       REG_NUM                                         :                        \
                    ((arg_type == CONST_ARG)? CUR_ARG(sizeof(float))                          :                        \
                                              NULL))))


COMMAND (push, 1, 1, ALL, {

    int arg_type = GET_ARG_TYPE(cur_cmd);

    if (arg_type == WRONG_ARG) {
        assert (0 && "Wrong push argument\n");
    }
    else {
        /* fixed_type tmp = *(GET_ARG(arg_type));
        printf("%f\n", tmp); */
        STACK_PUSH(GET_ARG(arg_type))
    }
)

COMMAND (pop, 2, 1, ALL, {

    int arg_type = GET_ARG_TYPE(cur_cmd);

    if (arg_type == WRONG_ARG) {
        assert (0 && "Wrong pop argument\n");
    }
    else {
        STACK_POP(GET_ARG(arg_type))
    }
)

//----------------------------------------------------other commands----------------------------------------------------

#if defined FLOAT
    #define CREATE_TMP_VALUES                                                                                          \
        float tmp_val1 = 0;                                                                                            \
        float tmp_val2 = 0;
 #else 
    #define CREATE_TMP_VALUES                                                                                          \
        int   tmp_val1 = 0;                                                                                            \
        int   tmp_val2 = 0;
#endif 

#define STACK_TOP(x)                                                                                                   \
    theStack.top(x);

#define CPU_POP(x)    STACK_POP(&tmp_val ## x)
#define CPU_TOP(x)    STACK_TOP(&tmp_val ## x)
#define CPU_PUSH(x)  STACK_PUSH(&tmp_val ## x)

COMMAND (hlt, 0, 0, NO, {  
    IP = file_size+1;
)

#if defined FLOAT
#define MATHS(x)                                                                                                       \
        CREATE_TMP_VALUES                                                                                              \
        CPU_POP(1)                                                                                                     \
        CPU_POP(2)                                                                                                     \
        tmp_val1=tmp_val2 x tmp_val1;                                                                                \ 
        CPU_PUSH(1);
#else
    #define MATHS(x)                                                                                                   \
        CREATE_TMP_VALUES                                                                                              \
        CPU_POP(1)                                                                                                     \
        CPU_POP(2)                                                                                                     \
        tmp_val1 = fixed_ ## x (tmp_val2, tmp_val1);                                                                   \ 
        CPU_PUSH(1);    
#endif

COMMAND (sub, 4, 0, NO, {
    IF_FIXED(MATHS(sub))
    IF_FLOAT(MATHS(-))
)

COMMAND (jmp, 10, 0, LABEL, {
    IP = (int)(*CUR_ARG(sizeof(float)));
)

COMMAND (add, 5, 0, NO, {
    IF_FIXED(MATHS(add))
    IF_FLOAT(MATHS(+))  
)

COMMAND (mul, 3, 0, NO, {
    // IF_FLXED(MATHS(mul))
    IF_FLOAT(MATHS(*))
)

COMMAND (div, 21, 0, NO, {
    IF_FIXED(MATHS(div))
    IF_FLOAT(MATHS(/))
)

COMMAND (out, 6, 0, NO, {
    CREATE_TMP_VALUES
    STACK_TOP(&tmp_val1)
    IF_FLOAT(printf("-------------------------------------float------------------------------------   %f\n",          (float)tmp_val1));
    IF_FIXED(printf("-------------------------------------fixed------------------------------------   %f\n", fixed_to_float(tmp_val1)));
)

//?
COMMAND (cmp, 17, 2, ALL, {

    CREATE_TMP_VALUES

    CF = 0;
    SF = 0;
    ZF = 0;

    int arg_type = GET_ARG_TYPE(cur_cmd);
    if (arg_type == WRONG_ARG) {
        assert (0 && "Wrong move argument\n");
    }
    else {
        tmp_val1 = *(GET_ARG(arg_type));

        cur_cmd = CUR_COMMAND;
        arg_type = GET_ARG_TYPE(cur_cmd);

        if (arg_type == WRONG_ARG) {
            assert (0 && "Wrong CMP argument\n");
        }
        else {
            tmp_val2 = *(GET_ARG(arg_type));
            tmp_val1 = tmp_val2 - tmp_val1;
            if (tmp_val1 == 0)    ZF = 1;
            else {
                if (tmp_val1 > 0) CF = 1;
                if (tmp_val2 < 0) SF = 1;
            }
        }
    }
)

COMMAND (move, 18, 2, ALL, {

    #if defined FLOAT
        float* dest = nullptr;
        float* src  = nullptr;
    #else
        int* dest   = nullptr;
        int* src    = nullptr;
    #endif


    int arg_type = GET_ARG_TYPE(cur_cmd);

    if (arg_type == WRONG_ARG) {
        assert (0 && "Wrong move argument\n");
    }
    else {
        src      =     GET_ARG(arg_type);

        cur_cmd  =           CUR_COMMAND;
        arg_type = GET_ARG_TYPE(cur_cmd);

        if (arg_type == WRONG_ARG) {

            assert (0 && "Wrong move argument\n");
        }
        else {

            dest  = GET_ARG(arg_type);
            *dest = *src;
        }
    }
)

#define IF_SYS_REG(reg, x)                                                                                             \
        if (reg == x) {                                                                                                \
            IP = *CUR_ARG(sizeof(int));                                                                                \
        }                                                                                                              \
        else IP += sizeof(int);

#define IF_SYS_REGS(reg1, x, reg2, y)                                                                                  \
        if ((reg1 == x)  || (reg2 == y)){                                                                              \
            IP = *CUR_ARG(sizeof(int));                                                                                \
        }                                                                                                              \
        else IP += sizeof(int);

COMMAND (ja, 11, 1, LABEL, {
    IF_SYS_REG(CF, 0);
)

COMMAND (jae, 12, 1, LABEL, {
    IF_SYS_REGS(CF, 0, ZF, 1);
)

COMMAND (jb, 13, 1, LABEL, {
    IF_SYS_REG(CF, 1);
)

COMMAND (jbe, 14, 1, LABEL, {
    IF_SYS_REGS(CF, 1, ZF, 1);
)

COMMAND (je, 15, 1, LABEL, {
    IF_SYS_REG(ZF, 1);
)

COMMAND (jne, 16, 1, LABEL, {
    IF_SYS_REG(ZF, 0);
)

COMMAND (call, 19, 1, LABEL, { // char int
    IF_FLOAT(float new_ip = IP + sizeof(int));
    IF_FIXED(int   new_ip = IP + sizeof(int));
    STACK_PUSH(&new_ip);
    IP = *CUR_ARG(sizeof(int));
)

COMMAND (ret, 20, 1, _RET_, {

    CREATE_TMP_VALUES
    tmp_val1 = *CUR_ARG(sizeof(int)); // num_of_pushed_args

    for (int i = 0; i < tmp_val1; i++) {
        STACK_POP(&tmp_val2)
    }
   
    STACK_POP(&tmp_val2)
    IP = (int)tmp_val2;
)

COMMAND (in, 22, 1, ALL, {
    CREATE_TMP_VALUES
    // printf("Enter: ");
    assert(scanf("%f", &tmp_val1) != 0);

    int arg_type = GET_ARG_TYPE(cur_cmd);

    if (arg_type == WRONG_ARG) {
        assert (0 && "Wrong IN argument\n");
    }
    else {
        *(GET_ARG(arg_type)) = tmp_val1;
    }
)

COMMAND (pow, 23, 1, ALL, {
    CREATE_TMP_VALUES

    int arg_type = GET_ARG_TYPE(cur_cmd);

    if (arg_type == WRONG_ARG) {
        assert (0 && "Wrong IN argument\n");
    }
    else {
        tmp_val1 = *(GET_ARG(arg_type));
    }
    CPU_POP(2)
    tmp_val2 = pow(tmp_val2, tmp_val1);
    CPU_PUSH(2)
)

COMMAND(sqrt, 7, 0, NO, {
        CREATE_TMP_VALUES                                                                  
        CPU_POP(1)                                                                                                                                                 \
        IF_FLOAT(tmp_val1 = sqrt(tmp_val1)); 
        IF_FIXED(tmp_val1 = fixed_sqrt(tmp_val1)); 
        CPU_PUSH(1);
)

COMMAND(draw, 24, 0, NO, {

    sf::RenderWindow window(sf::VideoMode(600,600), "Circle",sf::Style::Close);

    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        sf::RectangleShape r(sf::Vector2f(1,1));
        r.setFillColor(sf::Color::Red);

        int arr_p = 0;

        while (ram[arr_p+VM_OFFSET] != 0) {

            r.setPosition(ram[arr_p + VM_OFFSET], ram[arr_p+1+VM_OFFSET]);
            arr_p += 2;
            window.draw(r);
        }
        window.display();
    }
)  

COMMAND(meow, 25, 1, ALL, {

    CREATE_TMP_VALUES

    int arg_type = GET_ARG_TYPE(cur_cmd);
    // printf("MEOW\n");

    if (arg_type == WRONG_ARG) {
        assert (0 && "Wrong meow argument\n");
    }
    else {
        tmp_val1 = *(GET_ARG(arg_type));
        // int tmp = (int)tmp_val1;
        // printf("%f\n", tmp_val1);
        
        for (int i = 0; i < tmp_val1; i++) {
            printf("MOEW\n");
        }
    }
)

#undef CPU_PUSH
#undef CPU_POP
