#include <iostream>
#include <string>

#include <thread>
#include <mutex>
#include <condition_variable>


enum class Laundry {CLEAN, DIRTY};

std::mutex MUT;
Laundry SONS_LAUNDRY = Laundry::CLEAN;
std::condition_variable CV;

bool is_laundry_clean()
{
    return SONS_LAUNDRY == Laundry::CLEAN;
}

bool is_laundry_dirty()
{
    return SONS_LAUNDRY == Laundry::DIRTY;
}

void clean_laundry()
{
    std::unique_lock< std::mutex > lock(MUT);
    CV.wait(lock, is_laundry_dirty);
    
    std::cout << "Doing the son's laundry." << std::endl;
    SONS_LAUNDRY = Laundry::CLEAN;
    std::cout << "The laundry is clean." << std::endl;
    
    lock.unlock();
    CV.notify_one();
}


void play_around()
{
    std::cout << "Playing basketball and sweating." << std::endl;
    {
        std::lock_guard< std::mutex > lock(MUT);
        SONS_LAUNDRY = Laundry::DIRTY;
    }
    
    std::cout << "Asking mother to do the laundry." << std::endl;
    CV.notify_one();
    
    // waiting 
    {
        std::unique_lock< std::mutex > lock(MUT);
        CV.wait(lock, is_laundry_clean);
    }
    
    std::cout << "Yea, I have a clean laundry! Thank you mum!" << std::endl;
}


int main()
{

    std::thread mother(clean_laundry);
    std::thread son(play_around);
    
    mother.join();
    son.join();
    
    return 0;
}


