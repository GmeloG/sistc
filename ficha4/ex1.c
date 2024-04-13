#include <pthread.h>
#include <sys/types.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void myprint(const char *str);
void *mythread(void *);


int main()
{
  char *buf;
  pthread_t tid1, tid2;

  printf("Identificador do processo: %d\n", getpid());
  
  buf = (char *) malloc(256);
  sprintf(buf, "%d: Ola -------------------------------------------------------------\n",getpid());
  printf("Nova thread criada (tid1), fazendo a chamada à função mythread(buf) \n");
  pthread_create(&tid1, NULL, mythread, buf);

  //experimente também o programa com a linha seguinte comentada
  buf = (char *) malloc(256);
  sprintf(buf, "%d: Ole +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n",getpid());
  printf("Nova thread criada (tid2), fazendo a chamada à função mythread(buf) \n");
  pthread_create(&tid2, NULL, mythread, buf);

  printf("Pausa de 5 segundos na thread principal\n");
  sleep(5);
  
  printf("Thread principal vai aguardar que a thread identificada por tid1 termine.\n");
  //experimente também o programa com a linha seguinte comentada
  pthread_join(tid1, NULL);
      
  return(0);
}

void *mythread(void *arg){
  while(1) {
    sleep(1);

    myprint((char *)arg);
  }
}


/*.
1.1 - Execute o programa. Justifique o comportamento observado (strings “misturadas”) com 
base no descrito acima

1º caso string escreve as duas palavras ao mesmo tempo visto que estamos a alter a memoria do 
malocc antes de escrever a theard

2º caso buffer comentado

é escrito ole pk a string buf não é alterada, logo imprime as duas iguais pk estas partinham 
memoria

3º caso
o phttherad join comentado já não espera que este termine. logo avança para a proxima linha 
e termina o processo

1.2 Qual o objetivo da instrução pthread_join(tid1,NULL)?

A função pthread_join(tid1,NULL) espera que a theard1 termine. Mas como dentro do codigo 
da theard está um while(1) esta nunca termina.

1.3 -Tendo em conta as relações ilustradas, justifique a necessidade de fazer duas chamadas à 
função malloc no código em ex1.c. O que aconteceria caso se reutilizasse o bloco de 
memória alocado inicialmente? 
O Programa apenas imprimia a ultima string escrita visto que o bloco buf é partilhado entre theard
*/