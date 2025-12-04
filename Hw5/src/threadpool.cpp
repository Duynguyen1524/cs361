/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date September 2025
 @section Description

 Implementation of our Thread Pool.
 */

#include "threadpool.h"

ThreadPool::ThreadPool(taskPtr t){
    //Ask how many threads we have
    numThreads = std::thread::hardware_concurrency();
    //It is not a thread pool without at least 4
    //threads.
    if(numThreads < 4){
        numThreads = 4;
    }
    //All threads will be inactive at the start.
    status = std::vector<bool>(numThreads,false);
    //The queue starts empty
    todo = std::queue<taskPtr>();
    //Put in the first task
    todo.push(taskPtr(t));
}

void ThreadPool::run(){
    auto children = std::vector<std::thread>();
    for(int i=0; i < numThreads; i++){
        //Get the ref pointer to the method!
        children.emplace_back(&ThreadPool::workerThread,this,i);
    }
    for(auto& t: children){
        t.join();
    }
}

void ThreadPool::workerThread(int myID){
    //Local Variable this thread will need to manage the whole time
    //What task are we working on right now?
    taskPtr myTask;
    //What tasks do we need to tell everyone else about?
    std::queue<taskPtr> results;
    //Keep running until we are poisoned.
    while(true){
        //I need to get my task
        {std::unique_lock<std::mutex> lk(primaryLock);
            //I do not have a task right now.
            status[myID] = false;
            //Sleep until I have a task
            awaitTasks.wait(lk,[this]{return !todo.empty();});
            //Get my task
            myTask = todo.front();
            //Remove it from the list (consider it done, Sir!)
            todo.pop();
            //Tell everyone else that I have work to do!
            status[myID] = true;
        }
        //Complete the task outside of the lock
        try{
            //Try to do the task
            results = myTask->runTask();
        }catch(taskException e){
            //I was killed by poison!
            return;
        }
        //I did my job. I need to update the task board.
        //If I am the last living person then I shut down the pool.
        {std::unique_lock<std::mutex> lk(primaryLock);
            //Push all new tasks into the queue
            //local queue -> shared queue
            while(!results.empty()){
                todo.push(results.front());
                results.pop();
            }
            //Tell everyone if we have tasks to do.
            if(!todo.empty()){awaitTasks.notify_all();}
            //Check if we need to exit!
            //If
            //   there are no tasks
            //   and
            //   no one else is awake to make more tasks
            //then
            //   We are all done!
            if(todo.empty() && othersAsleep(myID)){
                //We have no work to do!
                for(int i=0; i < status.size(); i++){
                    todo.push(std::make_shared<poison>());
                }
                //Wakeup! The end has come for us all.
                awaitTasks.notify_all();
            }
            //Ok, I am going to "sleep" until I get my next task.
            status[myID] = false;
        }
    }
}

bool ThreadPool::othersAsleep(int myID){
    //Check that all values are false.
    //Should only be used if you already
    //hold the lock
    bool active = false;
    for(int i=0; i < status.size(); i++){
        if(i!=myID){
            active = status[i] || active;
        }
    }
    //If active is true, at least one thread is
    //doing something.
    //That means all others are not asleep.
    //If no one is active, we are the only awake thread.
    return !active;
}
