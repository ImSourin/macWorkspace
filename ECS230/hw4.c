#include <string.h>
#include "stdio.h"
#include "stdlib.h"

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

void printLowerTriangularMatrix(double *mat, int n, char *matname) {
    printf("Matrix %s: \n", matname);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            printf("%lf\t", mat[i + j * n]);
        }
        for (int j = i + 1; j < n; j++) {
            printf("%lf\t", 0.0);
        }
        printf("\n");
    }
    printf("-------------------------------------------\n");
}

// BLAS library function for generalized matrix multiplication
void dgemm_(char *TRANSA, char *TRANSB, int *M, int *N, int *K, double *ALPHA, double *A, int *LDA, double *B, int *LDB,
            double *BETA, double *C, int *LDC);

// BLAS library function for generalized matrix-vector multiplication
void dgemv_(char *TRANS, int *M, int *N, double *ALPHA, double *A, int *LDA, double *X, int *INCX,
            double *BETA, double *Y, int *INCY);

// LAPACK library function for Cholesky factor
void dpotrf_(char *UPLO, int *N, double *A, int *LDA, int *INFO);

// BLAS library function for solving triangular matrix equations
void
dtrsm_(char *SIDE, char *UPLO, char *TRANS, char *DIAG, int *M, int *N, double *ALPHA, double *A, int *LDA, double *B,
       int *LDB);

int main(int argc, char **argv) {
    FILE *fp;
    fp = fopen("data.dat", "r+");

    int degree = atoi(argv[1]); // degree of polynomial
    degree += 1; // number of data points needed to fit the polynomial
    int numOfDataPoints = degree + 1; // number of data points given ib the input

    fscanf(fp, "%d", &numOfDataPoints); // read number of data points

    double *dataX = (double *) malloc(numOfDataPoints * sizeof(double));
    double *dataY = (double *) malloc(numOfDataPoints * sizeof(double));

    // read data
    for (int i = 0; i < numOfDataPoints; i++) {
        fscanf(fp, "%lf %lf", &dataX[i], &dataY[i]);
    }
    fclose(fp);

    double *X = (double *) malloc(numOfDataPoints * degree * sizeof(double));
    double *Y = (double *) malloc(numOfDataPoints * sizeof(double));

    // create matrix X
    for (int i = 0; i < numOfDataPoints; i++) {
        X[i] = 1;
        for (int j = 1; j < degree; j++) {
            X[i + j * numOfDataPoints] = dataX[i] * X[i + (j - 1) * numOfDataPoints];
        }
    }

    memcpy(Y, dataY, numOfDataPoints * sizeof(double));


    printMatrix(X, numOfDataPoints, degree, "X");


    double *XTX = (double *) malloc((degree + 1) * (degree + 1) * sizeof(double));

    char TRANSA = 'T'; // 'N' for Not A Transpose
    char TRANSB = 'N'; // 'N' for Not B Transpose

    double alpha = 1.;
    double beta = 0.;

    int INCX = 1;
    int INCY = 1;


    // compute XTX
    dgemm_(&TRANSA, &TRANSB, &degree, &degree, &numOfDataPoints, &alpha, X, &numOfDataPoints, X, &numOfDataPoints,
           &beta, XTX, &degree);

    printMatrix(XTX, degree, degree, "XTX");

    double *XTY = (double *) malloc((degree + 1) * sizeof(double));

    // compute XTY
    dgemv_(&TRANSA, &numOfDataPoints, &degree, &alpha, X, &numOfDataPoints, Y, &INCX, &beta, XTY, &INCY);

    printMatrix(XTY, degree, 1, "XTY");

    char UPLO = 'L';
    int INFO = 0;

    double *L = (double *) malloc((degree + 1) * (degree + 1) * sizeof(double));

    memcpy(L, XTX, (degree + 1) * (degree + 1) * sizeof(double));

    // Compute Cholesky factorization Lower triangular matrix L such that XTX = LLT
    dpotrf_(&UPLO, &degree, L, &degree, &INFO);

    printLowerTriangularMatrix(L, degree, "L");

    char SIDE = 'L';
    char DIAG = 'N';

    TRANSA = 'N';

    double *B = (double *) malloc((degree + 1) * sizeof(double));

    memcpy(B, XTY, (degree + 1) * sizeof(double));

    UPLO = 'L';

    // Compute LB = XTY
    dtrsm_(&SIDE, &UPLO, &TRANSA, &DIAG, &degree, &degree, &alpha, L, &degree, B, &degree);

    TRANSA = 'T';

    // Compute LTB = B
    dtrsm_(&SIDE, &UPLO, &TRANSA, &DIAG, &degree, &degree, &alpha, L, &degree, B, &degree);

    printMatrix(B, degree, 1, "b");

    //write coefficients to file
    fp = fopen("out.dat", "w+");
    fprintf(fp, "%d\n", degree);
    for (int i = 0; i < degree; i++) {
        fprintf(fp, "%lf\n", B[i]);
    }
    fclose(fp);

    return 0;
}

