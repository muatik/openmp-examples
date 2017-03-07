#include <iostream>
#include <omp.h>

int main() {
    long num_steps = 1000;
    double step = 0;
    double x, pi, sum = 0.0;

    step = 1.0 / (double) num_steps;

    omp_set_num_threads(10);
    #pragma omp parallel for
    for (int i = 0; i < num_steps ; ++i) {
        x = (i + 0.5) * step;
#pragma omp critical
        {
            sum = sum + 4.0 / (1 + x * x);
        }
    }

    pi = step * sum;
    std::cout << pi;
    return 0;
}