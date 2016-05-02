#include <iostream>
#include <vector>
#include <omp.h>

void print_iterations(const std::vector<int>& iterations)
{
    int index = 0;
    for (const auto i : iterations)
    {
        std::cout << "Thread " << index++ << " -> " << i << " iterations" << std::endl;
    }
}


int main()
{
    std::vector<int> iterations(omp_get_max_threads(), 0);
    int n = 100;

    #pragma omp parallel shared(iterations, n)
    {    
        #pragma omp for 
        for (int i = 0; i < n; i++)
        {
            iterations[omp_get_thread_num()]++;
        }
    }

    print_iterations(iterations);
    return 0;
}


