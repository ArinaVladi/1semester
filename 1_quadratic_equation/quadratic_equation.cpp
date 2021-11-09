#include <stdio.h>
#include <math.h>                                                // разделить на файлы
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <float.h>

//-----------------------------------------------------------------------------

const double PRECISION        = 1.e-5;
const char*  ANSWER_INF_ROOTS = "Infinite number of roots";    // написать подсказки документации ко всем этим константам
const char*  ANSWER_NO_ROOTS  = "No roots";


enum rootOptions { INF_ROOTS = -1,
                   NO_ROOTS  =  0,
                   ONE_ROOT  =  1,
                   TWO_ROOTS =  2
};

//-----------------------------------------------------------------------------

struct EquationCoefficients {  //сказал оформить одну структуру с массивом внутри. Я не запомнила и почему-то не переспросила...
    double a;                  // назвать эту структуру полиномом
    double b;                  // ввести assert на степень полинома больше 2 или 3
    double c;
};

struct EquationRoots {
    int    nRoots;
    double x1;
    double x2;
};


//-----------------------------------------------------------------------------

/**
 * @brief:      coefficientsCheck ()
 *              Проверить, являются ли коэфиициенты конечными
 *
 * @param:      [in] ptr_coefficient - указатель на структуру,
 *              содержащую коэффициенты данного уравнения
 *
 * @detailed    Прервать выполнение программы макросом assert (),
 *              в случае неприемлимых коэффициентов
 */
void coefficientsCheck (struct EquationCoefficients* ptr_coefficients) {
    assert (isfinite (ptr_coefficients->a));
    assert (isfinite (ptr_coefficients->b));
    assert (isfinite (ptr_coefficients->c));
}


/**
 * @brief:   rootsCheck ()
 *           Проверить, являются ли коэфиициенты конечными
 *
 * @param:   [in] ptr_roots - указатель на структуру,
 *            содержащую сведения о корнях данного уравнения
 *
 * @detailed Прервать выполнение программы макросом assert (),
 *           в случае неприемлимых коэффициентов
 */
void rootsCheck (struct EquationRoots* ptr_roots) {
    assert (isfinite (ptr_roots->x1));
    assert (isfinite (ptr_roots->x2));
}


bool isZero (double val) {
    return (fabs (val) < PRECISION);
}

bool isLessThanZero (double val) {
    return (val < -PRECISION);
}


//-----------------------------------------------------------------------------


/**
 * @brief:   solveLinearEquation ()
 *           В случае, если коэффициент a = 0
 *           Решить линейное уравнение
 *
 * @param:   [in] ptr_coefficients - указатель на структуру,
 *           содержащую коэффициенты данного уравнения
 *
 *           [in] tr_roots - указатель на структуру,
 *           содержащую сведения о корнях данного уравнения
 *
 * @detailed Изменияет в структуре roots кол-во корней уравнения,
 *           если корней 1 или 2 изменяет поля со значением x1 или x1 и x2 соответственно
 */
void solveLinearEquation (struct EquationCoefficients* ptr_coefficients, struct EquationRoots* ptr_roots) {

    assert (ptr_coefficients);
    assert (ptr_roots);

    coefficientsCheck (ptr_coefficients);

    if  (isZero (ptr_coefficients->b)) {
        if (isZero (ptr_coefficients->c)) {
            ptr_roots->nRoots = INF_ROOTS;
        }
        else {
            ptr_roots->nRoots = NO_ROOTS;
        }
    }
    else {
        ptr_roots->x1 =- ptr_coefficients->c / ptr_coefficients->b;
        rootsCheck (ptr_roots);
            ptr_roots->nRoots = ONE_ROOT;
    }
}

/**
 * @brief:   solveQuadraticEquation ()
 *           В случае, если коэффициент a != 0
 *           Решить квадратное уравнение
 *
 * @param:   [in] ptr_coefficients - указатель на структуру,
 *           содержащую коэффициенты данного уравнения
 *
 *           [in] tr_roots - указатель на структуру,
 *           содержащую сведения о корнях данного уравнения
 *
 * @detailed Изменияет в структуре roots кол-во корней уравнения,
 *           если корней 1 или 2 изменяет поля со значением x1 или x1 и x2 соответственно
 */
void solveQuadraticEquation (struct EquationCoefficients* ptr_coefficients, struct EquationRoots* ptr_roots) {

    assert (ptr_coefficients);
    assert (ptr_roots);

    coefficientsCheck (ptr_coefficients);

    double discriminant = ptr_coefficients->b * ptr_coefficients->b - 4 * ptr_coefficients->a * ptr_coefficients->c;

    if (isLessThanZero (discriminant)) {
            ptr_roots->nRoots = NO_ROOTS;
    }

    else {
        if (isZero (discriminant)) {
            ptr_roots->x1 = (-ptr_coefficients->b) / (2 * ptr_coefficients->a);
            rootsCheck (ptr_roots);
            ptr_roots->nRoots = ONE_ROOT;
        }
        else {
            ptr_roots->x1 = (-ptr_coefficients->b - sqrt (discriminant)) / (2 * ptr_coefficients->a);
            ptr_roots->x2 = (-ptr_coefficients->b + sqrt (discriminant)) / (2 * ptr_coefficients->a);
            rootsCheck (ptr_roots);
            ptr_roots->nRoots = TWO_ROOTS;
       }
    }
}

/**
 * @brief:   printAnswer ()
 *           Вывести ответ уравнения в консоль
 *
 * @param:   [in] tr_roots - указатель на структуру,
 *           содержащую сведения о корнях данного уравнения
 *
 */
void printAnswer (struct EquationRoots* ptr_roots) {

    assert (ptr_roots != NULL);

    switch (ptr_roots->nRoots) {
        case INF_ROOTS:
             printf ("%s\n", ANSWER_INF_ROOTS);
             break;

        case NO_ROOTS:
             printf ("%s\n", ANSWER_NO_ROOTS);
             break;

        case ONE_ROOT:
             printf ("x = %g\n", ptr_roots->x1);
             break;

        case TWO_ROOTS:
             printf ("x1 = %f, x2 = %f\n", ptr_roots->x1, ptr_roots->x2);
             break;

        default:
             printf ("Expected %d, %d, %d or %d roots, but recieved %d roots\n\n", INF_ROOTS, NO_ROOTS, ONE_ROOT, TWO_ROOTS, ptr_roots->nRoots);  // выводить значение в nRoots
    }
}

/**
 * @brief:   nQuadraticEcquationRoots ()
 *           В зависимости от значения коэффициента a
 *           вызывает функцию решения линейного или квадратного ур-я
 *
 * @param:   [in] ptr_coefficients - указатель на структуру,
 *           содержащую коэффициенты данного уравнения
 *
 *           [in] tr_roots - указатель на структуру,
 *           содержащую сведения о корнях данного уравнения
 *
 */
void nQuadraticEcquationRoots (struct EquationCoefficients* ptr_coefficients, struct EquationRoots* ptr_roots) {

    assert (ptr_coefficients); // (ptr_coefficients != NULL);
    assert (ptr_roots);

    if (isZero (ptr_coefficients->a)) {
        solveLinearEquation (ptr_coefficients, ptr_roots);
    }
    else  {
        solveQuadraticEquation (ptr_coefficients, ptr_roots);
    }
    //printAnswer (ptr_roots);
}

/**
 * @brief:   inputCoefficients ()
 *           Ввод с клафиатуры коэффициентов уравнения
 *
 * @param:   [in] ptr_coefficients - указатель на структуру,
 *           содержащую коэффициенты данного уравнения
 *
 */
void inputCoefficients (struct EquationCoefficients* ptr_coefficients) {
    assert (ptr_coefficients);

    scanf ("%lf%lf%lf", &(ptr_coefficients->a), &(ptr_coefficients->b), &(ptr_coefficients->c));  //////////////////// форматный ввод

    coefficientsCheck (ptr_coefficients);
}


//-----------------------------------------------------------------------------


/**
 * @brief:   testEquationSolution ()
 *           Тестирует корректность выполнения программы
 *
 * @param:   [in] a - а-коэффициент ур-я
 *
 *           [in] b - b-коэффициент ур-я
 *
 *           [in] c - c-коэффициент ур-я
 *
 *           [in] expected_nRoots - правильное кол-во корней ур-я
 *
 *           [in] expected_x1     - правильный корень x1 ур-я
 *
 *           [in] expected_x2     - правильный корень x2 ур-я
 *
 */
void testEquationSolution (double a, double b, double c,
          int expected_nRoots, double expected_x1, double expected_x2) {

    static int testNumber=1;
    struct EquationCoefficients coefficients = {a, b, c};
    struct EquationRoots roots = {};

    nQuadraticEcquationRoots (&coefficients, &roots);

    rootsCheck (&roots);

    if ( (roots.nRoots != expected_nRoots) || (roots.x1 != expected_x1)){

        switch (roots.nRoots){
            case INF_ROOTS:
                printf ("Error in test %d\n"
               "Expected: %s, but received: %d roots\n\n",
               testNumber, ANSWER_INF_ROOTS, roots.nRoots);

            case NO_ROOTS:
                printf ( "Error in test %d\n"
                "Expected: %s, but received: %d roots\n\n",
                testNumber, ANSWER_NO_ROOTS, roots.nRoots);

            case ONE_ROOT:
                printf ("Error in test %d\n"
               "Expected % root x = %lg, but recieved: %d roots x1 = %lg, x2 = %lg\n\n",
               testNumber, expected_nRoots, expected_x1, roots.nRoots, roots.x1, roots.x2);

            case TWO_ROOTS:
                 printf ("Error in test %d\n"
               "Expected %d roots x1 = %lg, x2 = %lg, but recieved %d roots x1 = %lg, x2 = %lg\n\n",
               testNumber, expected_nRoots, expected_x1, expected_x2, roots.nRoots, roots.x1, expected_x2);

            default:
                printf ("Expected %d, %d, %d or %d roots, but recieved %d roots\n\n", INF_ROOTS, NO_ROOTS, ONE_ROOT, TWO_ROOTS, roots.nRoots);  // выводить значение в nRoots
        }
    }
    else {
        printf ("Test %d completed successfully\n\n", testNumber);
    }
    testNumber++;
}


void test(){



    //   a    b    c  num of roots  root1   root2
    testEquationSolution (0,   0,   0,    INF_ROOTS,     0,      0); // 1  inf roots
    testEquationSolution (0,   0,   3,            0,     0,      0); // 2  no roots: a=b=0
    testEquationSolution (0, 1.7, 3.4,            1,    -2,      0); // 3  x = -2 : a=0
    testEquationSolution (1,  -2,   2,            0,     0,      0); // 4  no roots: d<0
    testEquationSolution (1,  -2,   1,            1,     1,      1); // 5  x = 1 : d=0
    testEquationSolution (1,  17, -18,            2,    -18,     1); // 6  x1 = -18, x2 = 1 : d>0
}


//-----------------------------------------------------------------------------

#define TEST

//-----------------------------------------------------------------------------


int main (){

    #ifdef TEST
        test();

    #else

        struct EquationCoefficients coefficients = {};
        struct EquationRoots roots = {};

        inputCoefficients (&coefficients);
        nQuadraticEcquationRoots (&coefficients, &roots);
        printAnswer (&roots);

    #endif

    return 0;
}



