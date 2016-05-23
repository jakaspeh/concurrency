#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

class UniformDistribution
{
public:
    UniformDistribution() 
        : generator(),
          distribution(-1.0, 1.0)
    {
        int seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }

    double sample() 
    {
        return distribution(generator);
    }
    
    UniformDistribution(const UniformDistribution& ) = delete;
    UniformDistribution& operator()(const UniformDistribution& ) = delete;
    UniformDistribution(UniformDistribution&& ) = delete;
    UniformDistribution& operator()(UniformDistribution&& ) = delete;

private:
    std::default_random_engine generator;
    
    std::uniform_real_distribution<double> distribution;
};


double approximatePi(const int numSamples)
{
    UniformDistribution distribution;
    
    int counter = 0;
    for (int s = 0; s != numSamples; s++)
    {
        auto x = distribution.sample();
        auto y = distribution.sample();
        
        if (x * x + y * y < 1)
        {
            counter++;
        }
    }

    return 4.0 * counter / numSamples;
}

void fill_line(const int n)
{
    for (int i = 0; i != n; i++)
    {
        std::cout << "-";
    }
    std::cout << "\n";
}

int main()
{
    std::cout << "real Pi: 3.141592653589..." << std::endl;
    fill_line(40);

    int n = 1;
    for (int i = 1; i != 10; i++)
    {
        n *= 10;
        
        const auto approxPi = approximatePi(n);
        std::cout << "approx.: " 
                  << std::left
                  << std::setw(15) 
                  << std::setprecision(12) << approxPi 
                  << "  (" << n << " samples)" << std::endl;
    }

    fill_line(40);
    std::cout << "real Pi: 3.141592653589..." << std::endl;

    return 0;
}


