#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void myhandler(int signum);

int main() {
  int r;
  struct sigaction act;


  act.sa_handler=myhandler; // signal handler 
  sigemptyset(&act.sa_mask); //  limpar mask 
  act.sa_flags = 0;
  if(sigaction(SIGTERM, &act, NULL)==-1)
  {
    perror("sigaction");
    exit(1);
  };

  
  while(1){
    printf("Ola! Sou o processo %d. Mas pode confirmar com o comando ps.\n", getpid());
    r = sleep(10);
    if(r>0)
      printf("Sleep terminou %d segundos antes do instante esperado\n", r);
  }
         
  return 0;
}

void myhandler(int signum)
{
  printf("Recebi o sinal %d (%s). Mas nao quero terminar!\n", signum, strsignal(signum));
}

/*
  Ola! Sou o processo 226555. Mas pode confirmar com o comando ps.
  Ola! Sou o processo 226555. Mas pode confirmar com o comando ps.
  Recebi o sinal 15 (Terminated). Mas nao quero terminar!
  Sleep terminou 7 segundos antes do instante esperado
  Ola! Sou o processo 226555. Mas pode confirmar com o comando ps.

  O sinal não foi terminado 

  2.1) Analise o código do ficheiro ex2.c. Execute o programa ex2 em background. 
  Repita o procedimento da alínea 1.2. Justifique as diferenças de comportamento entre os 
  dois programas. 

  A diferença esta na função myhandler não tem um exit 
  para poder terminar este programa assim ao contrario 
  da anterior que exit um exit e termina logo as execuçoes
  
  2.2) Execute o comando kill -USR1 pid, onde pid é o identificador do processo 
  correspondente ao programa ex2. Explique o resultado.
  O progama e morto visto que não existe nenhuma declaração da extrução SIGUSR1,
  ele executa a extroção por defeito
   

*/