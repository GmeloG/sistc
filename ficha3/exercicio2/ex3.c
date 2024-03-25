#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void myhandler(int signum);
void print_pending();

int main() 
{
  int r;  
  struct sigaction act;
  sigset_t new_mask;


  //bloquear todos os sinais menos o SIGTERM e o SIGQUIT
  sigfillset(&new_mask);
  sigdelset(&new_mask, SIGTERM);
  sigdelset(&new_mask, SIGQUIT);
  if( sigprocmask(SIG_SETMASK, &new_mask, NULL) == -1)
  {
    perror("sigprocmask");
    exit(1);
  };


  act.sa_handler = myhandler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  if(sigaction(SIGTERM, &act, NULL)==-1)
  {
    perror("sigaction");
    exit(1);
  };
  if(sigaction(SIGQUIT, &act, NULL)==-1)
  {
    perror("sigaction");
    exit(1);
  };


  while(1){
    printf("Ola! Sou o processo %d. Mas pode confirmar com o comando ps.\n", getpid());
    print_pending();
    r = sleep(10);
    if(r>0)
      printf("Sleep terminou %d segundos antes do instante esperado\n", r);
  }
         
  return 0;
}

void myhandler(int signum)
{
  printf("Recebi o sinal %d (%s). Mas nao quero terminar!\n", signum, strsignal(signum));
  sleep(3); // se enviar o mesmo sinal novamente nesta altura ele ficará bloqueado até que a função myhandler termine. O que acontece se enviar o SIGTERM e logo de seguida o SIGQUIT?
  printf("Fim de myhandler (%d)\n", signum);
}

void print_pending()
{
  int signum;
  sigset_t set;

  sigpending(&set);

  for(signum = 1; signum < NSIG; ++signum)
    if(sigismember(&set, signum))
      printf("Sinal %d (%s) pendente\n", signum, strsignal(signum));
}

/*
    3.1) Execute o programa ex3 em background e envie-lhe os sinais SIGTERM e 
    SIGUSR1. Justifique as diferenças face às alíneas anteriores. 
    Ola! Sou o processo 250102. Mas pode confirmar com o comando ps.
      Ola! Sou o processo 250102. Mas pode confirmar com o comando ps.
      Ola! Sou o processo 250102. Mas pode confirmar com o comando ps.
      Ola! Sou o processo 250102. Mas pode confirmar com o comando ps.
      Sinal 28 (Window changed) pendente
      Ola! Sou o processo 250102. Mas pode confirmar com o comando ps.

      ^Z^Z
      ^C
      ^C
      ^C
      Ola! Sou o processo 250102. Mas pode confirmar com o comando ps.
      Sinal 2 (Interrupt) pendente
      Sinal 20 (Stopped) pendente

      Ola! Sou o processo 250102. Mas pode confirmar com o comando ps.
      Sinal 2 (Interrupt) pendente
      Sinal 20 (Stopped) pendente
      Sinal 28 (Window changed) pendente

    O programa não é terminado e todos os sinais ficam pedentes 
    e fica em buffer todos os sinais exectudados como pendestes

    3.2) Execute o comando kill –STOP pid, onde pid é o identificador do processo. Irá 
    observar que o processo deixa de imprimir mensagens no ecrã. Execute o comando ps –l e analise o resultado. Tenha em especial atenção a coluna S e a seguinte legenda 
    (extracto do man ps): 

    O processo ficou parado e não exectua mais

    3.3) Execute o comando kill –CONT pid, onde pid é o identificador do processo. 
Analise o resultado. 

  o processo começou a exectuar outravez

3.4) Termine o processo com o comando kill –KILL pid. 
*/