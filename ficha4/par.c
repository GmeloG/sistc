#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <float.h>
#include <pthread.h>

#define NUM_ELEM 200
#define NUM_ITER 14000000
#define NUM_THREADS 50

// Estrutura para passar argumentos para a função que vai ser executada pela theard
typedef struct
{
  double *ptr;
  int n;
} targs_t;

//prototipos
void operacao_muito_demorada(double *, int);
void print_statistics(time_t t0);
void print_data(double *dados);

// Função que vai ser executada pela theard
void *theardAux(void *args)
{
  targs_t *p = (targs_t *)args; // chamese cast
  operacao_muito_demorada(p->ptr, p->n);
  return NULL;
}

int main()
{

  // Allocation of a vector to hold the data to be processed
  double *dados = malloc(sizeof(double) * NUM_ELEM);
  if (dados == NULL)
  {
    perror("malloc");
    exit(1);
  }
  else
  {
    for (int i = 0; i < NUM_ELEM; ++i)
      dados[i] = i * 1.0;

    printf("%.1lf KiB allocated\n", sizeof(double) * NUM_ELEM / 1024.0);
  }

  // get current time, for benchmarking
  time_t t0;
  t0 = time(NULL);

  // operacao_muito_demorada(dados, NUM_ELEM);
  // operacao_muito_demorada(dados, NUM_ELEM/2);
  // operacao_muito_demorada(dados+NUM_ELEM/2, NUM_ELEM/2);

  pthread_t tids[NUM_THREADS];
  targs_t args[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++)
  {
    args[i].ptr = dados + i * (NUM_ELEM / NUM_THREADS);
    args[i].n = NUM_ELEM/NUM_THREADS ;
    pthread_create(&tids[i], NULL, theardAux,(void*)&args[i]); // criar theard
  }

  for (int i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(tids[i], NULL); //fechar o theard
  }

  print_statistics(t0);

  // this should print the sequence 0, 2, 4, ... , 2*(NUM_ELEM-1)
  print_data(dados);

  return 0;
}

void print_statistics(time_t t0)
{
  struct rusage usage;

  if (getrusage(RUSAGE_SELF, &usage) == -1)
    perror("getrusage");

  printf("%ld seconds elapsed\n", time(NULL) - t0);
  printf("user time: \t %3ld.%06ld s\n",
         usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
  printf("system time: \t %3ld.%06ld s\n",
         usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
  printf("total: %.3f seconds\n",
         usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1.0e6 +
             usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1.0e6);
}

void print_data(double *dados)
{
  int i;
  printf("\n");
  for (i = 0; i < NUM_ELEM - 1; ++i)
    printf("%.0lf, ", dados[i]);
  printf("%.0lf\n\n", dados[i]);
}

// Prenche o vector com dados desde o endereço base (vector) até vector+tamanho_vector-1
void operacao_muito_demorada(double *vector, int tamanho_vector)
{
  int i, j;
  double *v;
  printf("processing %d elements\n", tamanho_vector);
  double val = vector[0];

  // Apenas para consumir tempo
  for (j = 0; j < NUM_ITER; ++j)
  {
    v = vector;
    for (i = 0; i < tamanho_vector; ++i)
    {
      *v = drand48();
      v++;
    }
    v = vector;
    for (i = 0; i < tamanho_vector; ++i)
    {
      *v = sin(pow(*v, 3));
      v++;
    }
  }

  // Escrita no vector
  for (i = 0; i < tamanho_vector; ++i)
  {
    vector[i] = (val + i) * 2.0;
  }
}