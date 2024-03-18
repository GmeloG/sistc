#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h> 
#include <float.h>

//NELEM must be a multiple of 2
#define NELEM 20000000
#define NITER 46

void rectangular2polar(double *, double *, int); 
double mytime();

int main() { 
  double t0;  

  double *dados_in = (double *) malloc(sizeof(double)*NELEM); 
  double *dados_out = (double *) malloc(sizeof(double)*NELEM);
  if(dados_in == NULL || dados_out == NULL) { 
    perror("malloc"); 
    exit(1); 
  } else {
    
    //assign random initial values
    double maxv = DBL_MAX;
    srand48(time(NULL));
    for(int i = 0; i < NELEM; ++i)
      dados_in[i] = 2*(drand48()-0.5)*maxv;
  }

  //get current time, for benchmarking 
  t0 = mytime();   

  //This cycle is used only for benchmarking purposes
  for(int i=0; i < NITER; ++i)    
    rectangular2polar(dados_out, dados_in, NELEM);
  
  printf("Computation took %.1f s\n", mytime() - t0);
 
  return 0; 
} 

double mytime() {
  struct timeval tp;
  struct timezone tzp;

  gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}


/*Criar 30 processo criar 3 ciclos for */