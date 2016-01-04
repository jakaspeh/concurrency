#include <iostream>
#include <thread>

void printNTimes(int n, const std::string& msg)
{
    for (int i = 0; i != n; i++)
    {
        std::cout << msg << std::endl;
    }
}

int main()
{
    std::string msg1 = "Today is a beautiful day.";
    std::thread t1(printNTimes, 6, std::ref(msg1));
    
    std::string msg2 = "And sun is shining.";
    std::thread t2(printNTimes, 6, std::ref(msg2));
    
    t1.join();
    t2.join();
    
    return 0;
}


