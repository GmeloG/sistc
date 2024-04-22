#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>    /* For O_* constants */

void myprint(const char *str);
void *myfunc(void *);
sem_t *psem;

int main()
{
  char *buf = (char *)malloc(256);

  
  sem_unlink("/sem1"); // fechar o ficheiro do semaforo
  // criar um semáforo com o nome "/sem1" e inicializa-lo a 0
  psem = sem_open("/sem1", O_CREAT | O_RDWR, 0600, 1); // create se não existir cria se não não cria , read write, 0600 permissões 110 em, 1 valor inicial do semafaro.

  int r = fork();
  if (r == 0)
  {

    sprintf(buf, "%d: Ola -----------------------------------------\n", getpid());
    myfunc(buf);
  }
  else
  {
    sprintf(buf, "%d: Ole +++++++++++++++++++++++++++++++++++++++++\n", getpid());
    myfunc(buf);
    waitpid(r, NULL, 0);
  }

  return (0);
}

void *myfunc(void *arg)
{
  while (1)
  {
    
    sleep(1);
    sem_wait(psem); // Esta linha bloqueia o semáforo, até que o processo pai o liberte.
    myprint((char *)arg);
    sem_post(psem);
  }
}
