#include <iostream>
#include <string>
#include <queue>
#include <random>
#include <chrono>

#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex MUT;
std::queue <std::string> TASKS;
std::condition_variable CV;

int SEED = std::chrono::system_clock::now().time_since_epoch().count();

std::default_random_engine GENERATOR(SEED);
std::uniform_int_distribution<int> DISTRIBUTION(0, 4);
std::vector<std::string> TODO = {"Write program",
                                 "Fix bug",
                                 "Write unit tests",
                                 "Bring me coffee",
                                 "Clean my car"};

void add_task(const std::string& task)
{
    std::lock_guard<std::mutex> guard(MUT);
    std::cout << "Boss delegates: " << task << std::endl;
    TASKS.push(task);
    CV.notify_one();
}

void add_random_task()
{
    add_task(TODO[DISTRIBUTION(GENERATOR)]);
}

void go_home()
{
    add_task("go home");
}

void take_break()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
                                 
void boss()
{
    for (int i = 0; i != 5; i++)
    {
        add_random_task();
        take_break();
    }
    
    go_home();
}

void worker()
{
    while (true)
    {
        std::unique_lock<std::mutex> guard(MUT);
        CV.wait(guard, []{return !TASKS.empty();});
        std::string task = TASKS.front();
        TASKS.pop();
        
        std::cout << "Worker doing:   " << task << std::endl;
        
        if (task == "go home")
        {
            break;
        }
    }
}

int main()
{
    std::thread t1(boss);
    std::thread t2(worker);
    
    t1.join();
    t2.join();

    return 0;
}


