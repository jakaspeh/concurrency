#include <iostream>

int main()
{
    int i = 1;
    double d = 1.0;

    std::cout << std::is_same<int, decltype(i + i)>::value << "\n"
              << std::is_same<double, decltype(d + i)>::value << "\n"
              << std::is_same<int, decltype(d + i)>::value << "\n"
              << std::endl;

    std::cout << std::is_same<int, decltype(i)>::value << "\n"
              << std::is_same<int, decltype((i))>::value << "\n"
              << std::is_same<int&, decltype((i))>::value << "\n"
              << std::endl;

    return 0;
}



