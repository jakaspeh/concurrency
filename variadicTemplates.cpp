#include <iostream>

template <typename T>
void print(T value)
{
    std::cout << value << " ";
}

template <typename T, typename... Args>
void print(T first, Args... args)
{
    std::cout << first << " ";
    print(args...);
}

template <typename... Args>
void println(Args&&... args)
{
    print(std::forward<Args>(args)...);
    std::cout << std::endl;
}


int main()
{
    println(1);
    
    println(1, 2, 3, 4, 5);
    
    println("one", 2, "three", 4, "five");

    return 0;
}


