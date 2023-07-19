//
// Created by Sourin Chakrabarti on 11/5/22.
//
//
// Created by Sourin Chakrabarti on 11/5/22.
//

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
double *matmulDot1(int n, long *flop, double *a, double *b) {
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

/*
 * Compute the product of two matrices of size nxn stored in column major format using the dot product implementation
 */
double *matmulDot2(int n, long *flop, double *a, double *b) {
    double *result = (double *) malloc(n * n * sizeof(double));

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            result[i + j * n] = multiply(flop, a[i], b[j * n]);
            for (int k = 1; k < n; k++) {
                result[i + j * n] = add(flop, result[i + j * n], multiply(flop, a[i + k * n], b[k + j * n]));
            }
        }
    }

    return result;
}

/*
 * Compute the product of two matrices of size nxn stored in column major format using the gaxpy implementation
 */
double *matmulGaxpy1(int n, long *flop, double *a, double *b) {
    double *result = (double *) malloc(n * n * sizeof(double));

    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            result[j * n] = multiply(flop, a[k * n], b[k + j * n]);
            for (int i = 1; i < n; i++) {
                result[i + j * n] = add(flop, result[i + j * n], multiply(flop, a[i + k * n], b[k + j * n]));
            }
        }
    }

    return result;
}

/*
 * Compute the product of two matrices of size nxn stored in column major format using the gaxpy implementation
 */
double *matmulGaxpy2(int n, long *flop, double *a, double *b) {
    double *result = (double *) malloc(n * n * sizeof(double));

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            result[i] = multiply(flop, a[i + k * n], b[k]);
            for (int j = 1; j < n; j++) {
                result[i + j * n] = add(flop, result[i + j * n], multiply(flop, a[i + k * n], b[k + j * n]));
            }
        }
    }

    return result;
}

/*
 * Compute the product of two matrices of size nxn stored in column major format using the outer product implementation
 */
double *matmulOuter1(int n, long *flop, double *a, double *b) {
    double *result = (double *) malloc(n * n * sizeof(double));

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            result[i] = multiply(flop, a[i + k * n], b[k]);
            for (int j = 1; j < n; j++) {
                result[i + j * n] = add(flop, result[i + j * n], multiply(flop, a[i + k * n], b[k + j * n]));
            }
        }
    }

    return result;
}

/*
 * Compute the product of two matrices of size nxn stored in column major format using the outer product implementation
 */
double *matmulOuter2(int n, long *flop, double *a, double *b) {
    double *result = (double *) malloc(n * n * sizeof(double));

    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            result[j * n] = multiply(flop, a[k * n], b[k + j * n]);
            for (int i = 1; i < n; i++) {
                result[i + j * n] = add(flop, result[i + j * n], multiply(flop, a[i + k * n], b[k + j * n]));
            }
        }
    }

    return result;
}

int main(int argc, char **argv) {
    srand(time(NULL));// Fix the seed for generating random numbers
    int n = atoi(argv[1]);// Read n from arguments

    // Allocate memory to the two matrices
    double *a = (double *) malloc(n * n * sizeof(double));
    double *b = (double *) malloc(n * n * sizeof(double));

    //Initialize the two matrices
    initializeMatrix(n, a);
    initializeMatrix(n, b);

    // Labels and function pointer array for all 6 matrix multiplication loop orderings
    char *matmul_labels[] = {"Dot Product 1(i,j,k)", "Dot Product 2(j,i,k)", "Gaxpy 1(j,k,i)", "Gaxpy 2(i,k,j)",
                             "Outer Product 1(k,i,j)", "Outer Product 2(k,j,i)"};
    double *(*matmul_array[])(int, long *, double *, double *) = {matmulDot1, matmulDot2, matmulGaxpy1, matmulGaxpy2,
                                                                  matmulOuter1, matmulOuter2};


    int number_of_reps = 5; // Number of repetitions for each type for loop ordering

    for (int i = 0; i < 6; i++) {
        printf("%s :\n", matmul_labels[i]);
        printf("Time elasped\tNo. of floating point operations\tFloating point performance\n");
        double total_time = 0.0; // Total time across all repetitions
        long total_flop = 0; // Total floating point operations across all repetitions
        for (int j = 0; j < number_of_reps; j++) {
            long flop = 0; // Variable for counting the number of floating point operations
            double time = gtod(); // Variable for calculating the time taken for the multiplication
            double *product = (*matmul_array[i])(n, &flop, a, b); // matrix multiplication
            time = gtod() - time;
            printf("%lf s\t%ld\t\t\t\t\t%lf GFlop/s\n", time, flop, (1.0 * flop) / (time * 1e9));
            total_time += time;
            total_flop += flop;
        }
        printf("Average time - %lf s\n", total_time / number_of_reps);
        printf("Average Floating point performance - %lf GFlops/s\n", (1.0 * total_flop) / (total_time * 1e9));
    }


    return 0;

}