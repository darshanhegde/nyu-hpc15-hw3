/******************************************************************************
* FILE: omp_bug6.c
* DESCRIPTION:
*   This program compiles and runs fine, but produces the wrong result.
*   Compare to omp_orphan.c.
* AUTHOR: Blaise Barney  6/05
* LAST REVISED: 06/30/05
*  sum local variable defined within dotprod() function becomes a private variable
   because dotprod() function is called in the parallel reagion. But instead if the 
   shared variable sum is passed by reference everything should work fine except reduction
   can't work on pointers. So used a atomic section instead of reduction.
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define VECLEN 100

float a[VECLEN], b[VECLEN];

float dotprod(float* sum)
{
int i,tid;
    
tid = omp_get_thread_num();
#pragma omp for
  for (i=0; i < VECLEN; i++)
    {
    #pragma omp atomic
    *sum += (a[i]*b[i]);
    printf("  tid= %d i=%d\n",tid,i);
    }
}


int main (int argc, char *argv[]) {
int i;
float sum;

for (i=0; i < VECLEN; i++)
  a[i] = b[i] = 1.0 * i;
sum = 0.0;

#pragma omp parallel shared(sum)
  dotprod(&sum);

printf("Sum = %f\n", sum);

}

