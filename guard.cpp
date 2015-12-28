#include <iostream>
#include <thread>

class Guard
{
public:    
    Guard(std::thread& t)
        : thread(t)
    { }
    
    ~Guard()
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
    
    Guard(Guard& other)=delete;
    Guard& operator=(const Guard& rhs)=delete;

private:
    std::thread& thread;
};


void function()
{
    std::cout << "I'm inside function." << std::endl;
}


int main()
{
    std::thread t(function);
    Guard guard(t);

    return 0;
}

