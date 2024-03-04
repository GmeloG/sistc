/*
     
    a) Compile e teste o programa. Observe que ambos os ciclos for, nas linhas 17 e 25, 
    são executados em simultâneo. 
    b) Qual o valor de retorno da função fork no processo pai? Qual o significado deste 
    valor? 
    c) Por que motivo o printf da linha 13 é executado duas vezes? 
    d) Qual é o processo pai do processo criado para iniciar a execução deste programa? Que 
    tipo de programa é executado por esse processo? Confirme ou obtenha essa 
    informação através da execução (noutro terminal) do comando ps j enquanto o 
    programa faz as impressões.  
    e) Qual é o comportamento do programa se omitir a instrução exit(0); na linha 22?

*/

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
 
int main(int argc, char *argv[]) { 
  
  pid_t r;

  printf("\nProcesso %d a executar %s\n\n", getpid(), argv[0]); 
   
  r = fork(); //retorna 0 no novo processo 
 
  printf("Valor de retorno da função fork para o processo %d: %d\n\n",  getpid(), r); 
 
  if(r == 0) { 
    for(int i = 0; i < 10; ++i) { 
      sleep(1); 
      printf("Novo processo, com PID = %d (\"filho\" de %d)\n",  
        getpid(), getppid()); 
    } 
    exit(0); //termina o novo processo 
  } 
   
  for(int i = 0; i < 10; ++i) { 
    sleep(2);     
    printf("Processo inicial (PID = %d, \"filho\" de %d).\n",  
      getpid(), getppid()); 
  } 
 
  return(0); 
} 

/*
        b) 
            O valor retornado no processo pai é 49000 este valor diz o valor 
        do processo onde foi inicidado o programa.

        c)  
            printf é execudado duas vezes é exucutado tanto no processo pai como no filho
        
        d)
            PID (Process ID) e PPID (Parent Process ID)
            ps j: 
             PPID     PID    PGID     SID TTY        TPGID STAT   UID   TIME COMMAND
              816   75002   75002     816 pts/6      75002 S+    1000   0:00 ./ex1
            75002   75003   75002     816 pts/6      75002 S+    1000   0:00 ./ex1

            execução:

            Processo 75002 a executar ./ex1

            Valor de retorno da função fork para o processo 75002: 75003

            Valor de retorno da função fork para o processo 75003: 0

            Novo processo, com PID = 75003 ("filho" de 75002)
            Processo inicial (PID = 75002, "filho" de 816).
            Novo processo, com PID = 75003 ("filho" de 75002)
            Novo processo, com PID = 75003 ("filho" de 75002)
            Processo inicial (PID = 75002, "filho" de 816).
            Novo processo, com PID = 75003 ("filho" de 75002)
            Novo processo, com PID = 75003 ("filho" de 75002)
            Processo inicial (PID = 75002, "filho" de 816).
            Novo processo, com PID = 75003 ("filho" de 75002)
            Novo processo, com PID = 75003 ("filho" de 75002)
            Processo inicial (PID = 75002, "filho" de 816).
            Novo processo, com PID = 75003 ("filho" de 75002)
            Novo processo, com PID = 75003 ("filho" de 75002)
            Processo inicial (PID = 75002, "filho" de 816).
            Novo processo, com PID = 75003 ("filho" de 75002)
            Processo inicial (PID = 75002, "filho" de 816).
            Processo inicial (PID = 75002, "filho" de 816).
            Processo inicial (PID = 75002, "filho" de 816).
            Processo inicial (PID = 75002, "filho" de 816).
            Processo inicial (PID = 75002, "filho" de 816).

            o processo pai é 816 depois é criado o 75002 e 75003.


        e) Qual é o comportamento do programa se omitir a instrução exit(0); na linha 22? 
        executa o codigo do processo e pai e filho quando termina 
        o do filho executa a do processo pai 
*/