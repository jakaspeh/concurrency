#include <iostream>
#include <string>
#include <chrono>

#include <thread>
#include <future>
#include <utility>

class Car 
{
public:
    Car() 
        :hood(true), electricity(true),
         exhaust(true), engine(true)
    { }
    
    void remove_the_hood()
    { hood = false; }
    
    void add_the_hood()
    { hood = true; }

    void disconnect_electricity()
    { electricity = false; }

    void connect_electricity()
    { electricity = true; }

    void remove_exhaust_system()
    { exhaust = false; }
    
    void add_exhaust_system()
    { exhaust = true; }
    
    bool is_engine_ok() const
    { return engine; }

private:
    bool hood;
    
    bool electricity;
    
    bool exhaust;
    
    bool engine;
};

void make_break(int millisec)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
}

        
void mechanic(Car& car, std::promise<bool> engine_ok)
{
    car.remove_the_hood();
    car.disconnect_electricity();
    car.remove_exhaust_system();
    
    std::cout << "Mechanic: took a car apart." << std::endl;
    
    engine_ok.set_value(car.is_engine_ok());
    
    std::cout << "Mechanic: engine is ok." << std::endl;
    
    if (car.is_engine_ok())
    {
        make_break(10); 
        car.add_exhaust_system();
        car.connect_electricity();
        car.add_the_hood();
    
        std::cout << "Mechanic: put car back together." << std::endl;
    }
}


void manager()
{
    Car car;
    
    std::cout << "Manager: I would need to know if the car engine is ok."
              << std::endl;
    
    std::promise<bool> promise;
    std::future<bool> answer = promise.get_future();
    
    std::thread thread (mechanic, std::ref(car), std::move(promise));
    thread.detach();
    
    if (answer.get())
    {
        std::cout << "Manager: ensures the client that the engine is ok.\n"
                  << "Manager: negotiates the contract for selling the car."
                  << std::endl;
    }
    else
    {
        std::cout << "Manager: buys a new engine." << std::endl;
    }
    
    make_break(10);
}

int main()
{
    manager();
    return 0;
}

















