/*
 * dotblas.c: dot product using the BLAS library
 * The BLAS library is available on CSIF computers
 * Compile with gcc -o dotblas dotblas.c -lblas
 */

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "sys/time.h"

/* dot product */
/* BLAS ddot function prototype */
/* Note the added '_' character */
void
dgemm_(char *TRANSA, char *TRANSB, int *M, int *N, int *K, double *ALPHA, double *A, int *LDA, double *B, int *LDB,
       double *BETA, double *C, int *LDC);

double add(long* flop, const double x, const double y) {
    (*flop)++;
    return x + y;
}

double subtract(long* flop, const double x, const double y) {
    (*flop)++;
    return x - y;
}

double multiply(long* flop, const double x, const double y) {
    (*flop)++;
    return x * y;
}

double divide(long* flop, const double x, const double y) {
    (*flop)++;
    return x / y;
}

void initializeMatrix(int n, double *mat) {
    for (int i = 0; i < n * n; i++) {
        mat[i] = rand() % n + 1.0 * rand() / RAND_MAX;
    }
}

void printMatrix(int n, double *mat) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lf ", mat[i + j * n]);
        }
        printf("\n");
    }
}

double *matmulDot(int n, long* flop, double *a, double *b) {
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
    srand(time(NULL));
    int n = atoi(argv[1]);

//    double **a = (double **) malloc(n * sizeof(double*));
//    double **b = (double **) malloc(n * sizeof(double*));
//    double **result = (double **) malloc(n * sizeof(double*));
//
//    for(int i=0;i<n;i++) {
//        a[i] = (double *) malloc(n*sizeof (double));
//        b[i] = (double *) malloc(n*sizeof (double));
//        result[i] = (double *) malloc(n*sizeof (double));
//    }

//    initializeMatrix(n, a);
//    initializeMatrix(n, b);
//    initializeMatrix(n, result);

    double *a = (double *) malloc(n * n * sizeof(double));
    double *b = (double *) malloc(n * n * sizeof(double));
    double *res = (double *) malloc(n * n * sizeof(double));

    initializeMatrix(n, a);
    initializeMatrix(n, b);
    initializeMatrix(n, res);

    char TRANSA = 'N';
    char TRANSB = 'N';
    double alpha = 1.;
    double beta = 0.;
    int flop = 0;

    dgemm_(&TRANSA,&TRANSB,&n,&n,&n,&alpha,a,&n,b,&n,&beta,res,&n);

    printMatrix(n, matmulDot(n, &flop, a, b));
    printf("---------\n");
    printMatrix(n, res);
    return 0;
}
