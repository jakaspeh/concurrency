#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>

class Matrix
{
public:
    Matrix(int n, int m) :
        m_n(n),
        m_m(m),
        m_elements(n * m, 1)
    { }
    
    int operator()(int i, int j) const
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

int accumulate_row_wise(const Matrix& mat)
{
    auto sum = 0;
    auto rows = mat.get_rows();
    auto cols = mat.get_cols();

    for (auto r = 0; r != rows; r++)
    {
        for (auto c = 0; c != cols; c++)
        {
            sum += mat(r, c);
        }
    }
    
    return sum;
}

int accumulate_column_wise(const Matrix& mat)
{
    auto sum = 0;
    auto rows = mat.get_rows();
    auto cols = mat.get_cols();

    for (auto c = 0; c != cols; c++)
    {
        for (auto r = 0; r != rows; r++)
        {
            sum += mat(r, c);
        }
    }
    
    return sum;
}


int main()
{

    auto start = 1000;

    for (auto i = start; i != 11 * start; i += start)
    {
        Matrix m(i, i);
        
        auto t1_rows = std::chrono::steady_clock::now();
        auto sum_rows = accumulate_row_wise(m);
        auto t2_rows = std::chrono::steady_clock::now();

        auto t1_cols = std::chrono::steady_clock::now();
        auto sum_cols = accumulate_column_wise(m);
        auto t2_cols = std::chrono::steady_clock::now();

        using time_t = std::chrono::microseconds;
        auto time_rows = std::chrono::duration_cast<time_t>(t2_rows - t1_rows).count();
        auto time_cols = std::chrono::duration_cast<time_t>(t2_cols - t1_cols).count();
        auto factor = 1.0 * time_cols / time_rows;
        
        if (sum_rows == sum_cols)
        {
            std::cout << "#el = " << std::setw(9) << i * i 
                      << "  time-rows = " << std::setw(6) << time_rows
                      << "  time-cols = " << std::setw(6) << time_cols
                      << "  factor = " << std::setw(6) << factor
                      << std::endl;
        }
        else
        {
            std::cout << "results are wrong" << std::endl;
        }
    }

    return 0;
}


