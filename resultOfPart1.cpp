#include <iostream>

template <typename Fun, typename... Args>
void checkType(Fun f, Args... args)
{
    using return_t = typename std::result_of< Fun(Args...) >::type;

    std::cout << "int:    " << std::is_same<int, return_t>::value << "\n";
    std::cout << "double: " << std::is_same<double, return_t>::value << "\n";
    std::cout << "float:  " << std::is_same<float, return_t>::value << "\n";
    std::cout << std::endl;
}

int fun1(int i)
{
    return i;
}

double fun2(double d)
{
    return d;
}

float fun3(int i1, int i2)
{
    return static_cast<float>(i1 + i2);
}

int main()
{
    int i = 1;
    double d = 1.0;

    std::cout << "Function1: " << std::endl;
    checkType(fun1, i);

    std::cout << "Function2: " << std::endl;
    checkType(fun2, d);

    std::cout << "Function3: " << std::endl;
    checkType(fun3, i, i);

    return 0;
}


