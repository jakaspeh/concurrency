#include <iostream>
#include <string>
#include <chrono>
#include <queue>

#include <thread>
#include <future>
#include <utility>


std::mutex MUT;
std::queue < std::packaged_task<bool()> > ORDERS;
std::condition_variable CV;

bool CLOSED = false;

void make_break(const int milisec)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milisec));
}

void cooking()
{
    while (!CLOSED)
    {
        std::packaged_task< bool() > cooking_order;
        {
            std::unique_lock< std::mutex > guard(MUT);
            CV.wait(guard, []{return !ORDERS.empty();});
            cooking_order = std::move(ORDERS.front());
            ORDERS.pop();
        }
        cooking_order();
    }
}

bool cook_the_meal(const std::string meal)
{
    if (meal == "hamburger")
    {
        std::cout << "Cook: I don't make hamburgers!" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Cook: making the " << meal << std::endl;
        make_break(2);
        return true;
    }
}

std::future<bool> add_order(const std::string meal)
{
    std::cout << "Adding order: " << meal << std::endl;
    
    std::packaged_task<bool()> order(std::bind(cook_the_meal, meal));
    std::future< bool > result = order.get_future();

    std::lock_guard< std::mutex > guard(MUT);
    ORDERS.push(std::move(order));
    CV.notify_one();
    
    return result;
}

void serve(std::future<bool> meal, 
           const std::string mealName)
{
    if (meal.get())
    {
        std::cout << "Waiter: serving " << mealName << std::endl;
    }
    else
    {
        std::cout << "Waiter: Unfortunately we don't have " 
                  << mealName << ". Would you mind to order again?" 
                  << std::endl;
    }
}

void serve_orders()
{
    std::string mealOrder1 = "steak";
    std::string mealOrder2 = "hamburger";
    std::string mealOrder3 = "cheesecake";
    
    std::future<bool> meal1 = add_order(mealOrder1);
    std::future<bool> meal2 = add_order(mealOrder2);
    std::future<bool> meal3 = add_order(mealOrder3);
    
    serve(std::move(meal1), mealOrder1);
    serve(std::move(meal2), mealOrder2);
    serve(std::move(meal3), mealOrder3);
}

int main()
{
    std::thread cook(cooking);
    cook.detach();
    
    std::thread waiter(serve_orders);
    waiter.detach();
    
    
    make_break(100);
    CLOSED = true;
    
    return 0;
}




