//
//  Guass seidel iterations with red-black
//  parallelism
//
//  Created by Darshan Hegde on 4/21/15.

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "util.h"

// prints out the solution
void print_solution(double* u_k, int n){
    int i;
    for (i=0; i<n; i++) {
        printf("%f ", u_k[i]);
    }
    printf("\n");
}


// Does gauss-seidel iteration using OMP
void gauss_seidel_iteration(double* u_k, int N, int nthreads, int numIter){
    int i, tid, nperthread, iter;
    double first, last, u_prev;
    double a_inv = 0.5/pow((double)N+1, 2);
    double h_inv = pow((double)N+1, 2);
    nperthread = N/nthreads;
    tid = omp_get_thread_num();
    first = 0.0;
    last = 0.0;
    for (iter=0; iter<numIter; iter++) {
        // perform even iteration of gauss-seidel
        u_prev = first;
        for (i=tid*nperthread; i<(tid+1)*nperthread-1; i+=2) {
            u_k[i] = a_inv * (1+ h_inv*(u_prev+u_k[i+1]));
            u_prev = u_k[i-1];
        }
        // Wait for all threads to finish
#pragma omp barrier
        if (tid == 0) {
            first = 0.0;
            last = u_k[nperthread];
        } else if(tid == nthreads-1){
            first = u_k[tid*nperthread-1];
            last = 0.0;
        } else {
            first = u_k[tid*nperthread-1];
            last = u_k[(tid+1)*nperthread];
        }
        // Wait for all threads to exhange values
#pragma omp barrier
        // perform odd iteration of gauss-seidel
        u_prev = first;
        for (i=tid*nperthread+1; i<(tid+1)*nperthread-1; i+=2) {
            u_k[i] = a_inv * (1+ h_inv*(u_prev+u_k[i+1]));
            u_prev = u_k[i-1];
        }
        u_k[i] = a_inv * (1+ h_inv*(u_k[i-1]+last));
        // Wait for all threads to finish
#pragma omp barrier
        if (tid == 0) {
            first = 0.0;
            last = u_k[nperthread];
        } else if(tid == nthreads-1){
            first = u_k[tid*nperthread-1];
            last = 0.0;
        } else {
            first = u_k[tid*nperthread-1];
            last = u_k[(tid+1)*nperthread];
        }
        // Wait for all threads to exhange values
#pragma omp barrier
    }
}

int main(int argc, char** argv){
    timestamp_type time1, time2;
    
    if (argc != 3) {
        printf("USAGE: ./lapace-1d <Number of points (N)> <Num Iter>\n");
        abort();
    }
    int N = atoi(argv[1]);
    int numIter = atoi(argv[2]);
    double* u_k = (double*) malloc(N*sizeof(double));
    
    get_timestamp(&time1);
    //Initialize u_k
    int i;
    for (i=0; i<N; i++) {
        u_k[i] = 0.0;
    }
    
    int nthreads;
#pragma omp parallel
    {
        nthreads = omp_get_num_threads();
        // check if N is divisible by nthreads, otherwise abort
        if (N%nthreads!=0 || nthreads%2!=0) {
            printf("N should be multiple of nthreads. \n");
            abort();
        }
        
        int tid = omp_get_thread_num();
        printf("(%d) starting jacobi iteration. \n", tid);
#pragma omp barrier
        gauss_seidel_iteration(u_k, N, nthreads, numIter);
    }
    
    get_timestamp(&time2);
    double elapsed = timestamp_diff_in_seconds(time1,time2);
    printf("Time elapsed is %f seconds.\n", elapsed);

//    print_solution(u_k, N);
    free(u_k);
}