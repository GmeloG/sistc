/*
      1) Modifique o programa apresentado abaixo (sinais-iii.c) de forma a cumprir os
      requisitos seguintes:
      • O programa deverá inicialmente bloquear os sinais SIGINT e SIGQUIT.
      • Adicionalmente, os novos processos deverão ter o sinal SIGTSTP bloqueado.
      • O sinal SIGTERM deverá ser ignorado em todos os processos.
      • A função void myHandler(int signum) deverá ser definida como função de
      atendimento para o sinal SIGCHLD no processo inicial, antes de ser criado
      qualquer novo processo. Esse sinal deverá voltar a ter a sua configuração por
      omissão (default) nos processos criados pelo programa.

*/

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <wait.h>

void wait_something();
void do_something();
void myHandler(int);

int main()
{

  /* linha de identificação */
  printf("1211710 - %s\n", __FILE__);

  struct sigaction act;
  sigset_t new_mask;

  // bloquear SIGINT e SIQUIT
  sigemptyset(&new_mask);        // inicar marcara vazia
  sigaddset(&new_mask, SIGINT);  // colocar sinal sigint na mascara
  sigaddset(&new_mask, SIGQUIT); // colocar sinal sigquit na mascara
  if (sigprocmask(SIG_BLOCK, &new_mask, NULL) == -1)
  {
    perror("sigprocmask");
    exit(1);
  };

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
    wait_something();
    pid_t r = fork();
    if (r == 0)
    {
      // Bloquear SIGTSTP no processo filho
      sigemptyset(&new_mask);                  // incia a new_mask vazia
      sigaddset(&new_mask, SIGTSTP);           // adiciona o sinal sigtstp (paragem a lista)
      sigprocmask(SIG_BLOCK, &new_mask, NULL); // bloqueia o sinais que se encontra em new_mask neste caso é sigtsp

      // Ignorar SIGTERM no processo filho
      signal(SIGTERM, SIG_IGN); // ignora o sinal sigterm
      
      // Restaurar a configuração padrão para SIGCHLD no processo filho
      signal(SIGCHLD, SIG_DFL); // SIGCHLD para a ação padrão. SIGCHLD é o sinal enviado a um processo quando um processo filho termina.

      do_something();
      return (0);
    }
  }
  return (0);
}

void wait_something()
{
  sleep(1);
}

void do_something()
{
  sleep(1);
}

void myHandler(int signum)
{
  int wstatus;

  // espera que um processo filho termine, e ver o status e guarda o valor em wstatus

  wait(&wstatus);

  //  ve se o processo filho termeminou pelo sinal.
  if (WIFSIGNALED(wstatus))
  {
    printf("Child terminated by signal\n");
  }
}
