#include <iostream>
#include <chrono>
#include <thread>
#include <omp.h>

void function_1()
{
    for (int i = 0; i != 3; i++)
    {
        std::cout << "Function 1 (i = " << i << ")" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void function_2()
{
    for (int j = 0; j != 4; j++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "                   Function 2 "
                  << "(j = " << j << ")" << std::endl;
    }
}


int main()
{
    #pragma omp parallel sections
    {    
        #pragma omp section
        function_1();
            
        #pragma omp section
        function_2();
    }

    return 0;
}


