/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date September 2025
 @section Description
 
 A thread pool designed to generate every
 possible Tic-Tac-Toe board.
 
 This is for educational purposes. It is designed for clarity of understanding.
 */

#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include "task.h"
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

/**
 Use a thread pool to generate tic-tac-toe boards.
 */
class ThreadPool{
private:
    int numThreads;/**< How many threads we created.*/
    std::vector<bool> status;/**< Which threads are awake or asleep*/
    std::queue<taskPtr> todo;/**< What work is left?*/
    mutable std::mutex primaryLock;/**< Protect the queue*/
    std::condition_variable awaitTasks;/**< Threads with no work sleep.*/
public:
    /**
     Create a thread pool with the initial task.
     @param t is the first task to complete.
     */
    ThreadPool(taskPtr t);
    /**
     Start up all threads. This allows the programmer to control
     when the threads start.
     Blocks calling thread until pool is complete.
     */
    void run();
private:
    /**
     Define how a single worker processes tasks.
     @param myID is the array index code for the thread
     */
    void workerThread(int myID);
    /**
     Check if everyone but me is asleep. The calling thread is obviously awake.
     @param myID is the id to skip in the check
     */
    bool othersAsleep(int myID);
};

#endif
