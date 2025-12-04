/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date September 2025
 @section Description
 
Implementation of our Tasks.
 
Poison throws an exception for the thread to catch.
 */

#include "task.h"
#include <iostream>
#include <mutex>

std::queue<taskPtr> drawManagerTask::runTask() {
    std::queue<taskPtr> q;
    for (int r = 0; r < height; ++r) {
        q.push(std::make_shared<drawTask>(red, green, blue,height, r, x1, x2, y1, y2));
    }
    return q; 
}

std::queue<taskPtr> drawTask::runTask() {
    std::queue<taskPtr> q;
    for (int px = 0; px < width; ++px) {
        long double x = x1 + (x2 - x1) * px  / (width  - 1);
        long double y = y2 - (y2 - y1) * row / (height - 1);
        int iters = z(x, y, 100);
        if (iters == 100) {
            iters = 0; // black
        }
        unsigned char shade = (255 * iters / 100);
        int index = row * width + px;
        red[index] = green[index] = blue[index] = shade;
    }
    return q; // no new tasks from a leaf task
}

poison::poison(){
    //Nothing to do
}

std::queue<taskPtr> poison::runTask(){
    throw taskException();
}
