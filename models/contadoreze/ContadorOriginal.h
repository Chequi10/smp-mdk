#include "Smp/SimpleTypes.h"

#include <iostream>

// Definition of ContadorOriginal
class ContadorOriginal
{
    protected:
        Smp::Int64 Contador;     // Protected field to store Contador

    public:
        
        // Public methods to count, to reset and to return the Contador
        virtual void Punto(void);
        virtual void Reseteo(void);
        virtual Smp::Int64 get_Contador(void);

        // Public constructor.
        ContadorOriginal() : Contador(0) {}
};