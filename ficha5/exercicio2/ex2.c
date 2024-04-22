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

  // criar um semáforo com o nome "/sem1" e inicializa-lo a 0
  sem_t *psem = sem_open("/sem1", O_CREAT | O_RDWR, 0600, 0);

  int r = fork();
  if (r == 0)
  {

    sem_wait(psem); // Esta linha bloqueia o semáforo, até que o processo pai o liberte.
    sprintf(buf, "%d: Ola -----------------------------------------\n", getpid());
    myfunc(buf);
    sem_post(psem); // Esta linha liberta o semáforo, permitindo que o processo filho prossiga.
    exit(0);
  }
  else
  {
    sem_post(psem); // Esta linha liberta o semáforo, permitindo que o processo filho prossiga.
    sem_wait(psem); // Esta linha bloqueia o semáforo, até que o processo pai o liberte.
    sprintf(buf, "%d: Ole +++++++++++++++++++++++++++++++++++++++++\n", getpid());
    myfunc(buf);

    waitpid(r, NULL, 0);
  }

  sem_unlink("/sem1"); // Apagar o semáforo

  return (0);
}

void *myfunc(void *arg)
{
  while (1)
  {
    sleep(1);
    myprint((char *)arg);
  }
}
