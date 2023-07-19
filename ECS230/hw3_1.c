#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "sys/time.h"

/*
 * Measure time using gettimeofday library function
 */
volatile double gtod(void) {
    static struct timeval tv;
    static struct timezone tz;
    gettimeofday(&tv, &tz);
    return tv.tv_sec + 1.e-6 * tv.tv_usec;
}

/*
 * Add two double precision numbers and add a count to the number of floating point operations
 */
double add(long *flop, const double x, const double y) {
    (*flop)++;
    return x + y;
}

/*
 * Subtract two double precision numbers and add a count to the number of floating point operations
 */
double subtract(long *flop, const double x, const double y) {
    (*flop)++;
    return x - y;
}

/*
 * Multiply two double precision numbers and add a count to the number of floating point operations
 */
double multiply(long *flop, const double x, const double y) {
    (*flop)++;
    return x * y;
}

/*
 * Divide two double precision numbers and add a count to the number of floating point operations
 */
double divide(long *flop, const double x, const double y) {
    (*flop)++;
    return x / y;
}

/*
 * Initialize a matrix of size nxn with random double precision numbers
 */
void initializeMatrix(int n, double *mat) {
    for (int i = 0; i < n * n; i++) {
        mat[i] = rand() % n + 1.0 * rand() / RAND_MAX;
    }
}

/*
 * Initialize a matrix of size nxn stored in column major format
 */
void printMatrix(int n, double *mat) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lf ", mat[i + j * n]);
        }
        printf("\n");
    }
}

/*
 * Compute the product of two matrices of size nxn stored in column major format using the dot product implementation
 */
double *matmulDot(int n, long *flop, double *a, double *b) {
    double *result = (double *) malloc(n * n * sizeof(double));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i + j * n] = multiply(flop, a[i], b[j * n]);
            for (int k = 1; k < n; k++) {
                result[i + j * n] = add(flop, result[i + j * n], multiply(flop, a[i + k * n], b[k + j * n]));
            }
        }
    }

    return result;
}

int main(int argc, char **argv) {
    srand(time(NULL));// Fix the seed for generating random numbers
    int n = atoi(argv[1]); // Read n from arguments

    // Allocate memory to the two matrices
    double *a = (double *) malloc(n * n * sizeof(double));
    double *b = (double *) malloc(n * n * sizeof(double));

    //Initialize the two matrices
    initializeMatrix(n, a);
    initializeMatrix(n, b);

    long flop = 0; // Variable for counting the number of floating point operations
    double time = gtod(); // Variable for calculating the time taken for the multiplication
    double *product = matmulDot(n, &flop, a, b); // matrix multiplication
    time = gtod() - time;
    printf("Time elasped - %lf s\n", time);
    printf("No. of floating point operations - %ld\n", flop);
    printf("Floating point performance - %lf GFlop/s\n", (1.0 * flop) / (time * 1e9));

    return 0;

}