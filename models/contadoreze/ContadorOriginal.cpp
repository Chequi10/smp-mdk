// Include header files
#include "ContadorOriginal.h"

void ContadorOriginal::Punto(void)
{
    Contador++;
    std::cout << "Ezequiel Cuenta los " << Contador << std::endl;
}

void ContadorOriginal::Reseteo(void)
{
    Contador = 0;
}

Smp::Int64 ContadorOriginal::get_Contador(void)
{
    return Contador;
}