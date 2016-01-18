#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>

std::mutex m_print;
std::mutex m_bag;

void makeAppleJuice(std::vector<std::string>& bag)
{
    std::lock(m_print, m_bag);
    std::lock_guard<std::mutex> printGuard(m_print, std::adopt_lock);
    std::lock_guard<std::mutex> bagGuard(m_bag, std::adopt_lock);
    
    // std::lock_guard<std::mutex> printGuard(m_print);

    std::cout << "Making an apple juice..." << std::endl;

    // std::lock_guard<std::mutex> bagGuard(m_bag);
    
    int numApples = 0;
    for (std::size_t i = 0; i != bag.size(); i++)
    {
        if (bag[i] == "a")
        {
            numApples++;
            bag[i] = "x";
        }
    }
    
    if (numApples < 5)
    {
        std::cout << "I can not make an apple juice." << std::endl;
    }
    else
    {
        std::cout << "I made an excelent apple juice for you." << std::endl;
    }
}

void throwOutPear(std::vector<std::string>& bag)
{
    std::lock(m_print, m_bag);
    std::lock_guard<std::mutex> printGuard(m_print, std::adopt_lock);
    std::lock_guard<std::mutex> bagGuard(m_bag, std::adopt_lock);

    // std::lock_guard<std::mutex> bagGuard(m_bag);
    
    for (std::size_t i = 0; i != bag.size(); i++)
    {
        if (bag[i] == "p")
        {
            bag[i] = "x";
        }
    }
    
    // std::lock_guard<std::mutex> printGuard(m_print);

    std::cout << "I threw out all pears from you bag!" << std::endl;    
}

void printItem(const std::string& fruit)
{
    std::cout << fruit << " ";
}

int main()
{
    std::vector<std::string> bag = {"a", "a", "a", "p", "p", "a", "p", "a"};
    
    std::thread t1(makeAppleJuice, std::ref(bag));
    std::thread t2(throwOutPear, std::ref(bag));
    t1.join();
    t2.join();

    std::cout << "Bag: ";
    std::for_each(bag.begin(), bag.end(), printItem);
    std::cout << std::endl;
    
    return 0;
}


