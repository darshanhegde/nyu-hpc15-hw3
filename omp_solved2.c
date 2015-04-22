/******************************************************************************
* FILE: omp_bug2.c
* DESCRIPTION:
*   Another OpenMP program with a bug. 
* AUTHOR: Blaise Barney 
* LAST REVISED: 04/06/05 
* Bug: total variable needs to be in atomic block to avoid many threads overriting 
       the value. total needs to be promoted to a double variable so that answers 
       across different runs are same. Also, total is just printed out once at the 
       end of the code. tid needs to be a private variable, so it's decalration is 
       moved into parallel block.
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{
int nthreads, i;
double total=0.0;

/*** Spawn parallel region ***/
#pragma omp parallel 
  {
  int tid;
  /* Obtain thread number */
  tid = omp_get_thread_num();
  /* Only master thread does this */
  if (tid == 0) {
    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    }
  #pragma omp barrier
  printf("Thread %d is starting...\n",tid);
  #pragma omp barrier
      
  /* do some work */
  #pragma omp for schedule(dynamic, 10)
  for (i=0; i<1000000; i++){
     #pragma omp atomic
     total += i*1.0;
  }
  printf ("Thread %d is done! \n",tid);
  } /*** End of parallel region ***/
    
  printf("Total: %f \n", total);
}
