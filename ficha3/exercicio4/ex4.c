#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void myHandler(int signum)
{
  int wstatus;

  // espera que um processo filho termine, e ver o status e guarda o valor em wstatus
  wait(&wstatus);
  printf("Child terminated\n");
  //  ve se o processo filho terminou pelo sinal.
  if (WIFSIGNALED(wstatus))
  {
    printf("Child terminated by signal\n");
  }
}
void fun1(int *d)
{
  ++d[0];
  printf("Novo valor gerado em fun1: %d\n", d[0]);
  sleep(2);
}

void fun2(int *d)
{
  sleep(1);
  printf("Valor processado por fun2: %d\n", d[0]);
}

int main()
{
  int dados = 0;
  struct sigaction act;
  sigset_t new_mask;

  // Definir myHandler como o manipulador de sinal para SIGCHLD
  act.sa_handler = myHandler;
  sigemptyset(&act.sa_mask);                // esvaziar lista do manipulador para não começar com sinais bloqueados
  act.sa_flags = 0;                         // flags a 0
  if (sigaction(SIGCHLD, &act, NULL) == -1) // quando o processo ficho termina, se a chamada falhar returna o erro
  {
    perror("sigaction");
    exit(1);
  }

  while (1)
  {
    fun1(&dados);
    pid_t r = fork();
    if (r == -1)
    {
      perror("fork");
      exit(1);
    }
    if (r == 0)
    {
      
      fun2(&dados);
      exit(0);
    }
  }
}
