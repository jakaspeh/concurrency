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
            }
        }
    }

    return has_zero;
}

bool has_matrix_zero_entry_parallel (const Matrix& m)
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
                }
            }
        }
    }

    return has_zero;
}

int main()
{
    const int n = 1000;
    Matrix m(n, n);
    
    m(0, 10) = 0;
    m(100, 10) = 0;
    m(200, 10) = 0;
    m(300, 10) = 0;
    m(700, 10) = 0;
    m(800, 10) = 0;
    m(900, 10) = 0;
    
    auto time_seq = average_time(has_matrix_zero_entry_sequential, m, true);
    auto time_par = average_time(has_matrix_zero_entry_parallel, m, true);

    std::cout << "Sequential implementation: \n"
              << "Time: " << time_seq << "\n"
              << "Parallel implementation: \n"
              << "Time: " << time_par << std::endl;

    return 0;
}



