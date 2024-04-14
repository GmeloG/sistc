/*
    O processamento principal deste programa é feito na função rectangular2polar
(chamada na linha 35), que processa os valores em dados_in e guarda os respetivos
resultados em dados_out. Esta função está definida no ficheiro ex12-math.c.
A função mytime é usada para obter um valor aproximado do tempo de execução da
função rectangular2polar, cuja chamada é repetida várias vezes (ciclo for da linha
34) de forma a diminuir o impacto do erro de medida desse tempo.

a) Compile o programa através do seguinte comando:
cc -o ex12 ex12-main.c ex12-math.c -lm -O3
b) Teste o programa e altere o valor de NITER de modo a obter um tempo de execução
de aproximadamente 10 segundos.
c) Altere o programa anterior de modo que o processamento dos elementos em
dados_in pela função rectangular2polar seja dividido por 30 processos. Não
repita o código 30 vezes, use um ciclo for para criar os processos e outro ciclo for
para aguardar que cada um deles termine. A função rectangular2polar não deve
ser alterada.
Compile e teste o programa. O valor de NITER deverá ser o mesmo que usou na alínea
b). Relacione o tempo obtido com o número de núcleos de processamento
identificados pelo comando lscpu.


*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <float.h>
#include <wait.h>

// NELEM must be a multiple of 2
#define NELEM 20000000
#define NITER 46 // 46
#define Nprocs 1
void rectangular2polar(double *, double *, int);
double mytime();

int main()
{
  /* linha de identificação */
  printf("1211710 - %s\n", __FILE__);

  double t0;

  double *dados_in = (double *)malloc(sizeof(double) * NELEM);
  double *dados_out = (double *)malloc(sizeof(double) * NELEM);
  if (dados_in == NULL || dados_out == NULL)
  {
    perror("malloc");
    exit(1);
  }
  else
  {

    // assign random initial values
    double maxv = DBL_MAX;
    srand48(time(NULL));
    for (int i = 0; i < NELEM; ++i)
      dados_in[i] = 2 * (drand48() - 0.5) * maxv;
  }

  // get current time, for benchmarking
  t0 = mytime();
  int elems_per_proc = NELEM / Nprocs;
  
  pid_t r[Nprocs];

  // number of elements for each processor
  

  // number of process createad
  for (int j = 0; j < Nprocs; ++j)
  {
    // create a new processor
    r[j] = fork();
    // child process
    if (r[j] == 0)
    {
      for (int i = 0; i < NITER; ++i)
      {
        rectangular2polar(dados_out + elems_per_proc * i, dados_in + elems_per_proc * i, elems_per_proc);
      }
      exit(0);
    }
  }
  // parent process
  // wait for the child to finish
  for (int k = 0; k < Nprocs; ++k)
  {
    wait(NULL);
    //waitpid(r[k], NULL, 0);
  }

  printf("Computation took %.1f s\n", mytime() - t0);

  return 0;
}

double mytime()
{
  struct timeval tp;
  struct timezone tzp;

  gettimeofday(&tp, &tzp);
  return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}
