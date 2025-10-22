#include "mergesort.h"
#include <iostream>
#include <vector>

void mergesort(int* A, int size) {
  msortSec(A, 0, size - 1);
}
void msortSec(int* A, int start, int stop) {
  if (start < stop) {
    int middle = start + (stop-start) / 2;
    msortSec(A, start, middle);
    msortSec(A, middle + 1, stop);
    merge(A, start, middle, stop);
  }
}
void merge(int* A, int start, int middle, int stop){
    int* Aux = new int[stop - start + 1];
    for (int i = 0; i <= stop - start; i++) {
        Aux[i] = A[start + i];
    }
    int Aux_middle = middle - start;
    int Aux_stop = stop - start;
    int i = 0;
    int j = Aux_middle + 1;
    for (int k = start; k <= stop; k++){
        if (i > Aux_middle){
            A[k] = Aux[j];
            j++;
        }
        else if (j > Aux_stop){
            A[k] = Aux[i];
            i++;
        }
        else if (Aux[i] < Aux[j]){
            A[k] = Aux[i];
            i++;
        }
        else{
            A[k] = Aux[j];
            j++;
        }
    }
    delete[] Aux;
    
}