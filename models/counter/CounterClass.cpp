// Include header files
#include "CounterClass.h"

void CounterClass::Count(void)
{
    counter++;
   
    switch (counter)
    {
     case 1:printf ("\n rojo");
     break ;
     case 2: printf ("\n amarillo ");
     break ;
     case 3: printf("\n verde");
     break;
     default: CounterClass::Reset();
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