#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex increment;

void countEven(const std::vector<int>& numbers,
               int& numEven)
{
    for (const auto n : numbers)
    {
        if (n % 2 == 0)
        {
            increment.lock();
            numEven++;
            increment.unlock();
        }
    }
}

int main()
{
    std::vector<int> numbers1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> numbers2 = {11, 12, 13, 14, 15, 16, 17, 18};
    
    int n = 0;
    
    std::thread t1(countEven, std::ref(numbers1), std::ref(n));
    std::thread t2(countEven, std::ref(numbers2), std::ref(n));    

    t1.join();
    t2.join();
    
    std::cout << "Number of even numbers is: " << n << std::endl;
    return 0;
}

