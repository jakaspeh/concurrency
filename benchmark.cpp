#include <iostream>
#include <iomanip>

#include <limits>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>

#include <thread>
#include <future>


template <typename TimeT = std::chrono::microseconds>
class Benchmark
{
public:
    Benchmark(int num_iterations=100, int throw_away=0)
        : m_num_iter(num_iterations),
          m_throw_away(throw_away),
          m_times(),
          m_mean(),
          m_st_dev()
    { }

    template <typename Fun, typename... Args>
    std::vector< typename std::result_of< Fun(Args...) >::type > 
    benchmark(Fun&& fun, Args&&... args)
    {
        using result_t = typename std::result_of< Fun(Args...) >::type;
        
        std::vector< result_t > results;
        m_times.clear();
        auto n = m_num_iter + m_throw_away;

        for (auto i = 0; i != n; i++)
        {
            auto pair = measure_execution(std::forward<Fun>(fun), 
                                          std::forward<Args>(args)...);
            m_times.push_back(pair.first);
            results.push_back(pair.second);
        }
        
        compute_mean();
        compute_st_dev();
        
        return results;
    }

    typename TimeT::rep mean() const
    {
        return m_mean;
    }
    
    typename TimeT::rep standard_deviation() const
    {
        return m_st_dev;
    }
    

private:
    template <typename Fun, typename... Args>
    static std::pair< typename TimeT::rep,  typename std::result_of< Fun(Args...) >::type >
    measure_execution(Fun&& fun, Args&&... args)
    {
        auto t1 = std::chrono::steady_clock::now();
        auto result = std::forward<Fun>(fun)(std::forward<Args>(args)...);
        auto t2 = std::chrono::steady_clock::now();
    
        auto time = std::chrono::duration_cast<TimeT>( t2 - t1 ).count();
        return std::make_pair(time, result);
    }

    void compute_mean()
    {
        auto sum = std::accumulate(m_times.begin() + m_throw_away, m_times.end(), 0);
        m_mean = sum / m_num_iter;
    }

    void compute_st_dev()
    {
        std::vector<typename TimeT::rep> diff(m_num_iter);
        std::transform(m_times.begin() + m_throw_away, m_times.end(), diff.begin(),
                       [this](typename TimeT::rep t) {return t - this->m_mean;});
        
        auto sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0);
        m_st_dev = std::sqrt(sq_sum / m_num_iter);
    }
    
    int m_num_iter;
    
    int m_throw_away;

    std::vector<typename TimeT::rep> m_times;
    
    typename TimeT::rep m_mean;
    
    typename TimeT::rep m_st_dev;
};

int main()
{
    
    auto n = 1000000;
    std::vector<int> vector(n, 1);

    auto t1 = std::chrono::steady_clock::now();
    auto sum = std::accumulate(vector.begin(), vector.end(), 0);
    auto t2 = std::chrono::steady_clock::now();

    using nano_s = std::chrono::nanoseconds;
    using micro_s = std::chrono::microseconds;
    using milli_s = std::chrono::milliseconds;
    using seconds = std::chrono::seconds;
    using minutes = std::chrono::minutes;
    using hours = std::chrono::hours;

    auto d_nano = std::chrono::duration_cast<nano_s>( t2 - t1 ).count();
    auto d_micro = std::chrono::duration_cast<micro_s>( t2 - t1 ).count();
    auto d_milli = std::chrono::duration_cast<milli_s>( t2 - t1 ).count();
    auto d_s = std::chrono::duration_cast<seconds>( t2 - t1 ).count();
    auto d_m = std::chrono::duration_cast<minutes>( t2 - t1 ).count();
    auto d_h = std::chrono::duration_cast<hours>( t2 - t1 ).count();

    std::cout << "sum:      " << sum << "\n"
              << "d_nano:   " << d_nano << "\n"
              << "d_micro:  " << d_micro << "\n"
              << "d_milli:  " << d_milli << "\n"
              << "d_s:      " << d_s << "\n"
              << "d_m:      " << d_m << "\n"
              << "d_h:      " << d_h << "\n"
              << std::endl;


    for (auto numEl = n; numEl != 11 * n; numEl += n)
    {
        Benchmark<> benchmark;
        std::vector<int> vector(numEl, 1);

        using Iterator = decltype(vector.begin());
        auto results = benchmark.benchmark(std::accumulate< Iterator, int>, 
                                           vector.begin(), vector.end(), 0);
        
        
        std::cout << "#elements: " << std::setw(9) << numEl << " "
                  << " mean: " << std::setw(8) << benchmark.mean() 
                  << " st. dev: : " << std::setw(8) 
                  << benchmark.standard_deviation() 
                  << std::endl;

    }

    return 0;
}


