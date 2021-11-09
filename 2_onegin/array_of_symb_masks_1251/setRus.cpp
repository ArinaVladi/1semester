//
// Created by ArinaVladi on 20.09.2021.
//

#include "setRus.h"
//#include "macroUtilities.h"
void set_rus() {

   // Print_Debug("Russian is set");

    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
}

