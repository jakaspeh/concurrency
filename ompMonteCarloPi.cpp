#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <chrono>

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

int samplesInsideCircle(const int numSamples)
{
   UniformDistribution distribution;
    
    int counter = 0;
    for (int s = 0; s != numSamples; s++)
    {
        auto x = distribution.sample();
        auto y = distribution.sample();;
        
        if (x * x + y * y < 1)
        {
            counter++;
        }
    }
    
    return counter;
}

void sequentialPi()
{
    const int numSamples = 1000000000;
    const auto approxPi = approximatePi(numSamples);

    std::cout << "Sequential version: " << "\n";
    std::cout << "number of samples: " << numSamples << "\n";
    std::cout << "real Pi: 3.141592653589..." << "\n";
    std::cout << "approx.: " << std::setprecision(12) << approxPi << std::endl;
}

void parallelPi()
{
    
    const int numTotalSamples = 1000000000; 
    int numChunks = 8;
    int chunk = numTotalSamples / numChunks;
    
    int counter = 0;
    
#pragma omp parallel for shared(numChunks, chunk) reduction(+:counter)
    for (int i = 0; i < numChunks; i++)
    {
        counter += samplesInsideCircle(chunk); // maybee change to 
    }

    const double approxPi = 4.0 * counter / numTotalSamples;
    
    std::cout << "Parallel version: " << "\n";
    std::cout << "number of samples: " << numTotalSamples << "\n";
    std::cout << "real Pi: 3.141592653589..." << "\n";
    std::cout << "approx.: " << std::setprecision(12) << approxPi << std::endl;
}

int main()
{
    
    using millis = std::chrono::milliseconds;
    using std::chrono::duration_cast;
    using std::chrono::steady_clock;

    auto t_seq_1 = steady_clock::now();
    sequentialPi();
    auto t_seq_2 = steady_clock::now();

    auto time1 = duration_cast<millis>( t_seq_2 - t_seq_1 ).count();
    std::cout << "Time: " << time1 << " millisecons.\n" << std::endl;

    auto t_par_1 = steady_clock::now();
    parallelPi();
    auto t_par_2 = steady_clock::now();    

    auto time2 = duration_cast<millis>( t_par_2 - t_par_1 ).count();
    std::cout << "Time: " << time2 << " millisecons.\n" << std::endl;

    return 0;
}


