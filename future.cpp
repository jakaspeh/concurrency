#include <iostream>
#include <string>
#include <chrono>

#include <thread>
#include <future>

void make_break(int milisec)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milisec));
}


int temperature()
{
    std::cout << "Husband: Hm, is the weather "
              << "forecast in the newspaper?\n" 
              << "         Eh, we don't "
              << "have a newspaper at home..." << std::endl;
    
    make_break(2);
    
    std::cout << "Husband: I will look it up on the internet!" << std::endl;
    
    make_break(2);
    
    std::cout << "Husband: Here it is, "
              << "it says tomorrow will be 40." << std::endl;
    
    return 40;
}

int main()
{
    std::cout << "Wife:    Tomorrow, we are going on a picnic.\n" 
              << "         What will be the weather...\n" 
              << "         \"What will be the "
              << "temperature tomorrow?\"" << std::endl;
    
    
    std::future<int> answer = std::async(temperature);
    
    make_break(2);
    
    std::cout << "Wife:    I should pack for tomorrow." << std::endl;
    
    make_break(2);

    std::cout << "Wife:    Hopefully my husband can figure out the weather soon."
              << std::endl;
    
    int temp = answer.get();

    std::cout << "Wife:    Finally, tomorrow will be " << temp << "... Em...\n"
              << "         \"In which units is the answer?\"" 
              << std::endl;

    return 0;
}




