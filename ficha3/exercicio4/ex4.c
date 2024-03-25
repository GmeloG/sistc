#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void fun1(int *d) {
  ++d[0];
  printf("Novo valor gerado em fun1: %d\n", d[0]);
  sleep(2);
}

void fun2(int *d) {
  sleep(1); 
  printf("Valor processado por fun2: %d\n", d[0]);
}

int main(){
  int dados = 0;

  while(1) {
    fun1(&dados);
    pid_t r = fork();
    if(r == -1) {
      perror("fork");
      exit(1);
    }
    if(r==0) {
      fun2(&dados);
      exit(0);
    }
  }
}
