#include <iostream>
#include <thread>
#include <chrono>

void function_1()
{
    for (int i = 0; i != 4; i++)
    {
        std::cout << "Function 1 i = " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


void function_2()
{
    for (int j = 0; j != 4; j++)
    {
        std::cout << "                   Function 2 j = " 
                  << j << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


int main()
{

    std::thread thread_1(function_1);
    std::thread thread_2(function_2);
    
    thread_1.join();
    thread_2.join();
    
    return 0;
}


