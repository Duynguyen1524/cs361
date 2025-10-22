/**
 @file
 @author Justin Nguyen<jn867@drexel.edu>
 @date 10/17/2025
 @section Description
 This file implements the threaded merge sort. Threads are used to speed up the classic merge sort.
 */
#include "threadmerge.h"
#include <iostream>
#include <vector>
#include <thread>
#include "mergesort.h"

void helper(int* A,int size, int threadCount){
    std::vector<std::thread> threads;
    for (int i = 0; i < threadCount; i++) {
    int start = i * (size / threadCount);
    int end = (i == threadCount - 1) ? size : (i + 1) * (size / threadCount); // handle the last element of the segment. 
    threads.emplace_back(mergesort, A + start, end - start);
  }
  for(auto & t : threads){
      t.join();
  }

}
void helperMerge(int* A, int size, int threadCount){
    int segmentSize = size / threadCount;
    for (int i = 1; i < threadCount; i++) {
        int start = 0;
        int middle = i * segmentSize - 1;
        int end = (i == threadCount - 1) ? size - 1 : (i + 1) * segmentSize - 1;
        merge(A, start, middle, end);
    }

}
void tmergesort(int* A, int size, int threadCount){
  if (threadCount == 1 || size < threadCount) {
    mergesort(A, size);
    return;
  }
    helper (A, size, threadCount);
    helperMerge(A, size, threadCount);
  
}