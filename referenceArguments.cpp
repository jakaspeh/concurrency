#include <iostream>
#include <thread>
#include <algorithm>

void toUpper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

int main()
{
    std::string str = "Today is a beautiful day.";
    
    std::cout << "Before: " << str << std::endl;
    
    std::thread t(toUpper, std::ref(str));
    // std::thread t(toUpper, str); // try this instead of upper line
    t.join();
    
    std::cout << "After: " << str << std::endl;

    return 0;
}
    
    


