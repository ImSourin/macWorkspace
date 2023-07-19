#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void pr_mat(double *mat, int n,int p) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j){
            printf("%lf \t", mat[i + n * j]);
        }
        printf(" \n");
    }
    printf("-----------------\n");
}

void dgemv_(char *TRANSA, int *M, int *N, double *ALpHA, double *A, int *LDA, double *B, int *INCX, double* BETA, double *C, int *INCY
);

double dnrm2_(int *n,double *x,int *incx);

double gen_random(double num1,double num2){

    double den = RAND_MAX/(num2-num1);
    return num2 + (rand()/den);
}

int check(const double *uk,const double *uk1,int n){
    int flag=0;
    for(int i=0;i<n;i++){
        if(fabs(uk1[i]-uk[i])<0.000001){
            flag=1;
        }
        else flag=0;
    }

    return flag;
}

void power_method(double* uk,double* temp,double* uk1,double* a,int n,int cnt){
    char TRANSA='N';
    double ALPHA = 1.0;
    double BETA =0.0;
    int incx=1,incy=1,count=0;
    do{
        count++;
        memcpy(temp,uk,n*sizeof(double));
        dgemv_( &TRANSA, &n, &n, &ALPHA, a, &n, uk, &incx, &BETA, uk1, &incy);
        memcpy(uk,uk1,n*sizeof(double));
        if(count==cnt) break;
    }while(check(temp,uk1,n)==0);
}

double *get_x(int n,double *x,double sum){
    for(int i=0;i<n;i++){
        x[i]=x[i]/sum;
    }
    return x;
}

double sum_x(double* x,int n){
    int sum=0;
    for(int i=0;i<n+1;i++){
        sum=sum+(x[i]*x[i]);
    }
    return sqrt(sum);
}

int main() {

    int n=0,n1;


    FILE *fp;
    fp=fopen("hw5_5.in","r");
    if(fp==NULL) printf("file not found\n");
    else printf("file for Q3,Q4 opened successfully\n");

    fscanf(fp,"%d",&n);

    int* nonzero = (int*)malloc(n*sizeof(int));
    double* a = (double*)malloc(n*n*sizeof(double));
    double* uk = (double*)malloc(n*sizeof(double));
    double* uk1 = (double*)malloc(n*sizeof(double));
    double* temp = (double*)malloc(n*sizeof(double));
    for(int i=0;i<n;i++){
        fscanf(fp,"%d",&n1);
        for(int j=0;j<n1;j++){
            fscanf(fp,"%d",&nonzero[j]);
            a[nonzero[j]+i*n]=1.0/n1;
        }
    }
    printf("Printing a\n");
    pr_mat(a,n,n);
    for(int i=0;i<n;i++) {
        uk[i] = gen_random(1, 10);
    }
    power_method(uk,temp,uk1,a,n,2000);
    printf("Printing uk\n");
    pr_mat(uk,n,1);

    int N,N1;
    FILE *file;
    file=fopen("hw5_5.in","r+");
    if(file==NULL) printf("file not found\n");
    else printf("file for Q5 opened successfully\n");

    fscanf(file,"%d",&N);
    double* A = (double*)malloc(N*N*sizeof(double));
    int* nonzero1 = (int*)malloc(n*sizeof(int));
    for(int i=0;i<N;i++){
        //printf("le");
        fscanf(file,"%d",&N1);
        for(int j=0;j<N1;j++){
            //printf("te");
            fscanf(file,"%d",&nonzero1[j]);
            A[nonzero1[j]+i*N]=1.0/N1;
        }
    }
    printf("Printing a\n");
    pr_mat(A,N,N);
    for(int i=0;i<N;i++)
        uk[i]=1;
    power_method(uk,temp,uk1,A,N,2000);
    printf("Q5, u0={1,1,1,1}\n");
    pr_mat(uk,N,1);

    for(int i=0;i<N;i++){
        uk[i]=i+1;
    }

    power_method(uk,temp,uk1,A,N,2000);
    printf("Q5, u0={1,2,3,4}\n");
    pr_mat(uk,N,1);

    //Q6
    double* y = (double*)malloc(N*1*sizeof(double));
    double* x = (double*)malloc(N*1*sizeof(double));
    double* mu = (double*)malloc(N*1*sizeof(double));
    double* r = (double*)malloc(N*1*sizeof(double));
    double* mux = (double*)malloc(N*1*sizeof(double));
    double* sigmax = (double*)malloc(N*1*sizeof(double));
    double sigma=-0.95;
    double sum1=0,r2;

    for(int i=0;i<N;i++){
        x[i]=i+1;
        // printf("%lf\n",x[i]);
    }

    char TRANSA='N';
    char transa='T';
    double ALPHA = 1.0;
    double BETA =0.0;
    int incx=1,incy=1,p=1,count=0;
    int cnt =400;
    do{
        count++;
        double sum = dnrm2_(&N,x,&incx);
        printf("sum %lf\n",sum);
        x=get_x(N,x,sum);
        pr_mat(A,N,N);
        pr_mat(x,N,1);
        dgemv_( &TRANSA, &N, &N, &ALPHA, A, &N, x, &incx, &BETA, y, &incy);
//        dgemv_( &transa, &N, &p, &ALPHA, x, &N, y, &incx, &BETA, mu, &incy);
mu[0]=0;
        for(int i=0;i<N;i++) {
            mu[0] += x[i]*y[i];
            printf("xymu %lf %lf\n", x[i], y[i]);
        }
        printf("mu %lf\n",mu[0]);
        for (int i = 0; i < N; i++){
            mux[i] = x[i] * mu[0];
            sigmax[i]=x[i]*sigma;
        }
//        pr_mat(x,N,1);
        for(int i=0;i<N;i++){
            r[i]=y[i]-mux[i];
            x[i]=y[i]-sigmax[i];
            printf("%lf %lf %lf\n", r[i], y[i], sigmax[i]);
        }

        //sum1=sum_x(r,N);
        r2=dnrm2_(&N,r,&incx);
//        printf("%lf\n",r2);
        pr_mat(y,N,1);
        if(count==cnt) break;
    }while(r2>0.000001);

    printf("Q6, u0={1,2,3,4}\n");
    pr_mat(x,N,1);

    return 0;
}