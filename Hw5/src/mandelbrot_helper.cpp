#include "mandelbrot_helper.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <complex>
int z(long double x, long double y, int t) {
    
    std::complex<long double> c(x, y);
    std::complex<long double> current(0, 0);

    
    for (int i = 1; i < t; i++) {
        current = current * current + c;
        if (std::abs(current) > 4) {
            return i;
        }
    }

    
    return t;
}