// Include header files
#include "CounterClass.h"

void CounterClass::Count(void)
{   counter++;
    
    if(estado == 1)
    {    if(counter <= rojo)
            {estado=1;
            
            std::cout << "rojo " << counter << std::endl;
            }
            else
            {
                estado=2;
                
            }
    }
    else if(estado==2)
    {estado = 2;
        if(counter <= amarillo)
            {
                estado=2;
            
            std::cout << "amarillo " << counter << std::endl;
            }
            else
           {estado=3;}
    }       
    else if (counter <= verde)
    {            
            std::cout << "verde " << counter << std::endl;

    }
    else
    {
        estado=1;
        counter=0;
    }
              
}

void CounterClass::Reset(void)
{
    counter = 0;
}

Smp::Int64 CounterClass::get_Counter(void)
{
    return counter;
}