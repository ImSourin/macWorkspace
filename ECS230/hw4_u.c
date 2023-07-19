#include <stdio.h>
#include "stdlib.h"
#include "math.h"
#include "string.h"

void print_matrix(double *A, int n, int m) {

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            printf("%15.6f\t", A[i + n * j]);
        }
        printf("\n");
    }
}


void print_lt_matrix(double *A, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (i >= j) {
                printf("%15.6f\t", A[i + n * j]);
            } else {
                printf("%15.6f\t", 0.0);
            }
        }
        printf("\n");
    }

}

void dgemm_(char *TRANSA, char *TRANSB, int *M, int *N, int *K, double *ALPHA, double *A, int *LDA,
            double *B, int *LDB, double *BETA, double *C, int *LDC);

void dgemv_(char *TRANS, int *M, int *N, double *ALPHA, double *A, int *LDA,
            double *X, int *INCX, double *BETA, double *Y, int *INCY);

void dpotrf_(char *UPLO, int *N, double *A, int *LDA, int *INFO);

void
dtrsm_(char *SIDE, char *UPLO, char *TRANSA, char *DIAG, int *M, int *N, double *ALPHA, double *A, int *LDA, double *B,
       int *LDB);

void print_lt_matrix(double *pDouble, int i, int i1);

int main(int argc, char *argv[]) {
    int n;
    int data_pts = 0;
    FILE *f;
    if (argc == 1) {
        n = 3;
    } else {
        n = atoi(argv[1]);
    }
    int deg = n + 1;
    if (!(f = fopen("data.dat", "r+"))) {
        perror("Data File Open Error");
        exit(1);
    }
    fscanf(f, "%d", &data_pts);
    double *X_Data = (double *) malloc(data_pts * (sizeof(double)));

    double *Y = (double *) malloc(data_pts * (sizeof(double)));



    for (int i = 0; i < data_pts; i++) {
        fscanf(f, "%lf %lf", &X_Data[i], &Y[i]);
    }
    printf("\n_____________________ Printing XDat ___________________________\n");
    for (int k = 0; k < data_pts; k++) {

        printf("%lf\n", X_Data[k]);
    }
    printf("\n_____________________ Printing Y ___________________________\n");
    for (int k = 0; k < data_pts; k++) {

        printf("%lf\n", Y[k]);
    }
    fclose(f);
    double *X = (double *) malloc(data_pts * n * (sizeof(double)));//datapts,n
    for (int i = 0; i < data_pts; i++) {
        X[i] = 1;
        for (int j = 1; j < n + 1; j++) {
            X[i + j * data_pts] = X_Data[i] * X[i + (j - 1) * data_pts]; //pow(X_Data[i],(j-1));//* X[i+(j-1)*datapts
        }
    }
    printf("\n_____________________ Printing X ___________________________\n");
    print_matrix(X, data_pts, deg);

    double ALPHA = 1.0;
    double BETA = 0.0;
    char TRANSA = 'T';
    char TRANSB = 'N';
    double *XTRANSX = (double *) malloc((n + 1) * (n + 1) * (sizeof(double)));
    dgemm_(&TRANSA, &TRANSB, &deg, &deg, &data_pts, &ALPHA, X, &data_pts, X, &data_pts, &BETA, XTRANSX, &deg);
    //deg,deg,daata_pts,
    //dgemv - deg,datapts,1,1
    printf("\n_____________________ Printing XTRANSX _____________________\n");
    print_matrix(XTRANSX, deg, deg);
    double *L = (double *) malloc((n + 1) * (n + 1) * (sizeof(double)));

    int INCX = 1;
    int INCY = 1;
    char TRANS = 'T';
    double *XTRANSY = (double *) malloc((n + 1) * (sizeof(double)));//deg+1
    dgemv_(&TRANS, &data_pts, &deg, &ALPHA, X, &data_pts, Y, &INCX, &BETA, XTRANSY, &INCY);
    printf("\n_____________________ Printing XTRANSY _____________________\n");
    print_matrix(XTRANSY, n + 1, 1);
    int INFO = 0;
    char UPLO = 'L';
    memcpy(L, XTRANSX, (n + 1) * (n + 1) * (sizeof(double)));
    dpotrf_(&UPLO, &deg, L, &deg, &INFO);
    printf("\n_____________________ Printing L ___________________________\n");
    print_lt_matrix(L, n + 1, n + 1);
    char SIDE = 'L';
    UPLO = 'L';
    char DIAG = 'N';
    TRANSA = 'N';
    int col = 1;
    double *B = (double *) malloc((n + 1) * (sizeof(double)));
    memcpy(B, XTRANSY, (n + 1) * (sizeof(double)));
    dtrsm_(&SIDE, &UPLO, &TRANSA, &DIAG, &deg, &col, &ALPHA, L, &deg, B, &deg);
    printf("\n_____________________ Printing B ___________________________\n");
    print_matrix(B, deg, 1);
    TRANSA = 'T';
    dtrsm_(&SIDE, &UPLO, &TRANSA, &DIAG, &deg, &col, &ALPHA, L, &deg, B, &deg);
    printf("\n_____________________ Printing B ___________________________\n");
    print_matrix(B, deg, 1);
    return (0);


}

