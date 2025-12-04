/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date September 2025
 @section Description
 
 An abstract task for our thread pool.
 Along with the game task and
 poison task.
 The poison task is used to kill a thread.
 */

#ifndef _TASK_H_
#define _TASK_H_

#include <queue>
#include <string>
#include <memory>
#include "mandelbrot_helper.h"


//I need to tell the compiler tasks exist
//because tasks to contain other task pointers.
//I don't want to type
//std::queue<std::shared_ptr<task>> x
//a dozen times.
class task;
/**
 Giving a short name to our task pointers.
 */
typedef std::shared_ptr<task> taskPtr;


/**
 A general abstract task for the thread pool to use.
 */
class task{
public:
    //These are virtual functions
    //They need to be implemented
    //Returns any new tasks to do
    /**
     Run a task. This should be done in a thread.
    */
    virtual std::queue<taskPtr> runTask() = 0;
    /**
     Destructor to remove a task from memory
    */
    virtual ~task(){}
};


/**
This class represents ManagerTask to assign task to workers thread.
*/
class drawManagerTask : public task {
public:
    /**
    The constructor for the drawManagerTask.
    @param r is the red pixel array
    @param g is the green pixel array
    @param b is the blue pixel array
    @param h is the height of the image
    @param x1_ is the minimum x coordinate
    @param x2_ is the maximum x coordinate
    @param y1_ is the minimum y coordinate
    @param y2_ is the maximum y coordinate  
    */
    drawManagerTask(unsigned char* r, unsigned char* g, unsigned char* b,
                    int h,
                    long double x1_, long double x2_,
                    long double y1_, long double y2_)
    {
        red = r; green = g; blue = b; height = h;
        x1 = x1_; x2 = x2_; y1 = y1_; y2 = y2_;
    }

    /**
    Redefination of runTask to create drawManagerTasks to assign tasks for each row.
    @return A queue of drawTasks for each row.
    */
    std::queue<taskPtr> runTask();
private:
    unsigned char* red;/**< The value of the red component of the pixel.*/
    unsigned char* green;/**< The value of the green component of the pixel.*/
    unsigned char* blue;/**< The value of the blue component of the pixel.*/
    int height; /**< The height of the image.*/
    long double x1; /**< The minimum x coordinate. */
    long double x2; /**< The maximum x coordinate. */
    long double y1; /**< The minimum y coordinate. */
    long double y2; /**< The maximum y coordinate. */
};
class drawTask: public task{
public:
    /**
     The constructor for the drawTask.
     @param r is the red pixel array
     @param g is the green pixel array
     @param b is the blue pixel array
     @param h is the height of the image
     @param rowIndex is the specific row this task will draw
     @param x1_ is the minimum x coordinate
     @param x2_ is the maximum x coordinate
     @param y1_ is the minimum y coordinate
     @param y2_ is the maximum y coordinate  
     */
    drawTask(unsigned char* r, unsigned char* g, unsigned char* b,
             int h, int rowIndex,
             long double x1_, long double x2_,
             long double y1_, long double y2_)
         {
        red = r; green = g; blue = b; height = h; row = rowIndex;
        x1 = x1_; x2 = x2_; y1 = y1_; y2 = y2_;
         }
    /**
     Redefination of runTask to draw a specific row of the image.
     @return An empty queue as this is a leaf task.
     */
    std::queue<taskPtr> runTask();
private:
    unsigned char* red; /**< The value of the red component of the pixel.*/
    unsigned char* green; /**< The value of the green component of the pixel.*/
    unsigned char* blue; /**< The value of the blue component of the pixel.*/
    int height; /**< The height of the image. */
    int row; /**< The specific row this task will draw. */
    const int width = 1500; /**< The width of the image. */
    long double x1;/**< The minimum x coordinate. */
     long double x2;/**< The maximum x coordinate. */
      long double y1; /**< The minimum y coordinate. */
      long double y2; /**< The maximum y coordinate. */
};

/**
 Custom exception to use when no new tasks remain.
 */
class taskException : public std::exception {
private:
    std::string message;/**< Message to display in error.*/
public:
    /**
     Create a new exception with default message.
     */
    taskException(){message="No Tasks Remain.";}
    /**
     This defines how the exception is displayed as a string.
     @return a message that no tasks remain.
     */
    const char* what() const noexcept {
        return message.c_str();
    }
};

/**
 A special task to exit threads.
 */
class poison: public task{
public:
    /**
     Create a poison task to cause a thread to exit.
     */
    poison();
    /**
     Throws an exception for the thread to catch.
     @return never used.
     */
    std::queue<taskPtr> runTask();
};
#endif
