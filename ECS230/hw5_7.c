//
// Created by Sourin Chakrabarti on 11/28/22.
//
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "stdbool.h"

#define ERR 1e-6
#define MAX_ITERATIONS 1e3

// BLAS library function for generalized matrix-vector multiplication
void dgemv_(char *TRANS, int *M, int *N, double *ALPHA, double *A, int *LDA, double *X, int *INCX,
            double *BETA, double *Y, int *INCY);

// BLAS library function for calculating norm
double dnrm2_(int *N, double *x, int *INCX);

// Printing matrix
void printMatrix(double *mat, int n, int m, char *matname) {
    printf("Matrix %s: \n", matname);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%lf\t", mat[i + j * n]);
        }
        printf("\n");
    }
    printf("-------------------------------------------\n");
}

// Printing vector
void printVector(double *vec, int n, char *vecname) {
    printf("Vector %s: \n", vecname);
    for (int i = 0; i < n; i++) {
        printf("%lf\t", vec[i]);
    }
    printf("\n");
    printf("-------------------------------------------\n");
}

// Random vector initialization
void initializeVector(int n, double *vec) {
    for (int i = 0; i < n; i++) {
        vec[i] = rand() % n + 1.0 * rand() / RAND_MAX;
    }
}

// Vector initialization with all 1s. For question 5
void initializeVectorWith1(int n, double *vec) {
    for (int i = 0; i < n; i++) {
        vec[i] = 1.0;
    }
}

// Vector initialization with 1,2,3...n. For question 5
void initializeVectorSequential(int n, double *vec) {
    for (int i = 0; i < n; i++) {
        vec[i] = i + 1.0;
    }
}

// Normalize vector
void normalize(double *vec, int n) {
    int INCX = 1;
    double norm = dnrm2_(&n, vec, &INCX);

    for (int i = 0; i < n; i++) vec[i] /= norm;
}

// Convergence condition
bool hasChanged(double *vec1, double *vec2, int n) {
    for (int i = 0; i < n; i++) {
        if (fabs(vec1[i] - vec2[i]) >= ERR) return true;
    }
    return false;
}

int main(int argc, char **argv) {
    srand(time(NULL));
    FILE *fp;
    fp = fopen(argv[1], "r+");

    int n;
    fscanf(fp, "%d", &n);
    double *A = (double *) malloc(n * n * sizeof(double));
    // Creating matrix A
    for (int i = 0; i < n; i++) {
        int ni;
        fscanf(fp, "%d", &ni);
        for (int j = 0; j < ni; j++) {
            int pos;
            fscanf(fp, "%d", &pos);
            A[pos + i * n] = 1.0 / ni;
        }
    }

    double m = 0.15;

    //creating matrix M = (1-m)A + mS
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            A[i+j*n] = (1.0-m)*A[i+j*n] + m/n;
        }
    }

    printMatrix(A, n, n, "A");

    double *uk = (double *) malloc(n * sizeof(double));
    double *uk1 = (double *) malloc(n * sizeof(double));

//    initializeVector(n, uk1);
    initializeVectorWith1(n, uk1);
//    initializeVectorSequential(n, uk1);

    char TRANSA = 'N'; // 'N' for Not A Transpose
    double alpha = 1.;
    double beta = 0.;
    int INCX = 1;
    int INCY = 1;
    int iter = 1;

    // Normalized power method
    do {
        memcpy(uk, uk1, n * sizeof(double));
        dgemv_(&TRANSA, &n, &n, &alpha, A, &n, uk, &INCX, &beta, uk1, &INCY);
        normalize(uk1, n);
        printf("Iteration %d:\n", iter);
        printVector(uk1, n, "eigenvector for eigenvalue=1");
        iter++;
        if (iter >= MAX_ITERATIONS) break;
    } while (hasChanged(uk1, uk, n));

    printVector(uk1, n, "Final eigenvector for eigenvalue=1");
    fclose(fp);
    return 0;
}