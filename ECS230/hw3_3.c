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

// BLAS library function for generalized matrix multiplication
void dgemm_(char *TRANSA, char *TRANSB, int *M, int *N, int *K, double *ALPHA, double *A, int *LDA, double *B, int *LDB,
            double *BETA, double *C, int *LDC);

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

int main(int argc, char **argv) {
    srand(time(NULL));// Fix the seed for generating random numbers
    int n = atoi(argv[1]);// Read n from arguments

    // Allocate memory to the two matrices and the result
    double *a = (double *) malloc(n * n * sizeof(double));
    double *b = (double *) malloc(n * n * sizeof(double));
    double *res = (double *) malloc(n * n * sizeof(double));

    //Initialize all the matrices
    initializeMatrix(n, a);
    initializeMatrix(n, b);
    initializeMatrix(n, res);

    char TRANSA = 'N'; // 'N' for Not A Transpose
    char TRANSB = 'N'; // 'N' for Not B Transpose

    // res = aplha*A*B + beta*res
    double alpha = 1.;
    double beta = 0.;

    double time = gtod(); // Variable for calculating the time taken for the multiplication
    dgemm_(&TRANSA, &TRANSB, &n, &n, &n, &alpha, a, &n, b, &n, &beta, res, &n);
    time = gtod() - time;

    printf("Time elasped - %lf s\n", time);

    return 0;
}
