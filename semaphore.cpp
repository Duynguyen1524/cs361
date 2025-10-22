/**
 * @file semaphore.cpp
 * @author Justin Nguyen
 * @date 2024-10-04
 * @section This is the semaphore interface from the mulitprogrmming assignment.
 */
#include <mutex>
#include <condition_variable>
/**
 * This is the semaphore interface from the mulitprogrmming assignment.
 * You can modify this file as you wish.
 * You can add any functions and variables you need.
 */
class semaphore
{
private:
    unsigned int count;// count of the semaphore
    std::mutex mtx; //lock
    std::condition_variable cv;// sleep until the count is > 0

public:
    //create a semaphore with initial count = 1;
    semaphore(/* args */);
    semaphore(unsigned int c);
    void signal();
    void wait();

    ~semaphore();
};

semaphore::semaphore(/* args */)
{
}

semaphore::~semaphore()
{
}
 Semaphore {