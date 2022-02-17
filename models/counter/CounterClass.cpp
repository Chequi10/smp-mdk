// Include header files
#include "CounterClass.h"

void CounterClass::Count(void)
{
    counter++;
    std::cout << "otra forma " << counter << std::endl;
}

void CounterClass::Reset(void)
{
    counter = 0;
}

Smp::Int64 CounterClass::get_Counter(void)
{
    return counter;
}