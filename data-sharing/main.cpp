#include <iostream>
#include <omp.h>

void compare_cases();
using namespace std;

double G = 2.1;

int main() {
    compare_cases();
    return 0;
}

void compare_cases() {
    int a=1, b=2, c=3, t=4;
    omp_set_num_threads(3);
    #pragma omp parallel private(a), firstprivate(b)
    {
        // a will be private and, but not be initialized
        // b will be private and initialized
        // c will be shared;
        // t will be local; that is it is private.
        int t = 5;

        static int s = 8; // will be shared.
        if (omp_get_thread_num() == 0)
            s = 2;

        printf("thread id: %d, a: %d, b: %d, c: %d, t: %d, s: %d, G: %f, \n", omp_get_thread_num(), a, b, c, t, s, G);
        a = 21;
        b = 22;
        c = 23;
        t = 24;
    }

    printf("\nout of the parallel region\n");
    printf("thread id: %d, a: %d, b: %d, c: %d, t: %d, G: %f, \n", omp_get_thread_num(), a, b, c, t, G);

//    OUTPUT:
//    thread id: 0, a: 0, b: 2, c: 3, t: 5, s: 2, G: 2.100000,
//    thread id: 2, a: 32752, b: 2, c: 3, t: 5, s: 2, G: 2.100000,
//    thread id: 1, a: 32767, b: 2, c: 3, t: 5, s: 2, G: 2.100000,
//
//    out of the parallel region
//    thread id: 0, a: 1, b: 2, c: 23, t: 4, G: 2.100000,
}
