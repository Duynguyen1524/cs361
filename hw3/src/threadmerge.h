/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date 2021-2022
 @section Description
 
 This file describes the threaded merge sort. Threads are used to speed up the classic merge sort.
 */

//--You may add to this file, but you may not change the templates provided--

#ifndef _THREAD_MERGE_H_
#define _THREAD_MERGE_H_

/**
 Merge Sort array in place using maximum number of threads provided. If number of elements is smaller than number of threads, extraneous threads are not spanned.
 @param A is the array to sort
 @param size is the number of elements in the array
 @param threadCount is the maximum number of threads to use when sorting.
 
 */
void tmergesort(int* A, int size, int threadCount);

/**
 helper is a function that spawns threads to sort segments of the array in parallel.
 @param A is the array to sort
 @param size is the number of elements in the array
 @param threadCount is the maximum number of threads to use when sorting.
 
 */
void helper(int* A,int size, int threadCount);
/**
 helperMerge is a function that merges the sorted segments of the array.
 @param A is the array to sort
 @param size is the number of elements in the array
 @param threadCount is the maximum number of threads to use when sorting.
 
 */
void helperMerge(int* A, int size, int threadCount);

#endif
