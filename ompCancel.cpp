#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <omp.h>

class Matrix
{
public:
    Matrix(int n, int m) :
        m_n(n),
        m_m(m),
        m_elements(n * m, 1)
    { }
    
    int& operator()(int i, int j) 
    {
        return m_elements[i * m_m + j];
    }

    const int& operator()(int i, int j) const
    {
        return m_elements[i * m_m + j];
    }

    int get_cols() const
    {
        return m_m;
    }
    
    int get_rows() const
    {
        return m_n;
    }

private:
    int m_n;

    int m_m;
    
    std::vector<int> m_elements;
};

template <typename T>
double time_diff(const T& start, const T& end)
{
    std::chrono::duration<double> time = end - start;
    return time.count();
}

template <typename Fun>
double average_time(Fun&& fun, const Matrix& matrix, const bool expected_result)
{
    int numRep = 10;
    double sum = 0;
    for (int r = 0; r != numRep; r++)
    {
        auto t1 = std::chrono::steady_clock::now();
        auto result = fun(matrix);
        auto t2 = std::chrono::steady_clock::now();
        sum += time_diff(t1, t2);

        if (result != expected_result)
        {
            std::cout << "Result is wrong." << std::endl;
        }
    }
    
    return sum / numRep;
}



bool has_matrix_zero_entry_sequential(const Matrix& m)
{
    const auto rows = m.get_rows();
    const auto cols = m.get_cols();

    bool has_zero = false;
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if (m(row, col) == 0)
            {
                has_zero = true;
                break;
            }
        }
        
        if (has_zero)
        {
            break;
        }
    }

    return has_zero;
}


bool has_matrix_zero_entry_parallel_1(const Matrix& m)
{
    const auto rows = m.get_rows();
    const auto cols = m.get_cols();

    bool has_zero = false;
#pragma omp parallel default(none) shared(m, has_zero)
    {
#pragma omp for 
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if (m(row, col) == 0)
                {

#pragma omp critical
                    {
                        has_zero = true;
                    }
#pragma omp cancel for 
                }
            }
        }
    }

    return has_zero;
}

bool has_matrix_zero_entry_parallel_2(const Matrix& m)
{
    const auto rows = m.get_rows();
    const auto cols = m.get_cols();

    bool has_zero = false;
#pragma omp parallel default(none) shared(m, has_zero)
    {
#pragma omp for 
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if (m(row, col) == 0)
                {

#pragma omp critical
                    {
                        has_zero = true;
                    }
#pragma omp cancel for 
                }
            }
#pragma omp cancellation point for
        }
    }

    return has_zero;
}



int main()
{
    const int n = 8000;
    Matrix m(n, n);
    
    m(7000, 10) = 0;
    // m(0, 100) = 0;
    
    auto time_seq = average_time(has_matrix_zero_entry_sequential, m, true);
    auto time_par_1 = average_time(has_matrix_zero_entry_parallel_1, m, true);
    auto time_par_2 = average_time(has_matrix_zero_entry_parallel_2, m, true);

    std::cout << "Does OpenMP allow cancel construct: " 
              << omp_get_cancellation() << std::endl;

    std::cout << "Sequential implementation: \n"
              << "Time: " << time_seq << "\n"
              << "Parallel implementation (without cancellation point): \n"
              << "Time: " << time_par_1 << "\n"
              << "Parallel implementation (with cancellation point): \n"
              << "Time: " << time_par_2 << std::endl;

    return 0;
}


// $ ./ompCancel 
// Does OpenMP allow cancel construct: 0
// Sequential implementation: 
// Time: 0.03917
// Parallel implementation (without cancellation point): 
// Time: 0.0351697
// Parallel implementation (with cancellation point): 
// Time: 0.031086

// $ export OMP_CANCELLATION=true
// $ ./ompCancel 
// Does OpenMP allow cancel construct: 1
// Sequential implementation: 
// Time: 0.0412532
// Parallel implementation (without cancellation point): 
// Time: 0.0188502
// Parallel implementation (with cancellation point): 
// Time: 1.81924e-05

