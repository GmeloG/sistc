/*
    Crie uma nova versão do programa elaborado no exercício anterior em que a execução 
    do comando “ps -f” é feita através duma chamada a uma das funções da família exec 
    (e.g., execlp), em substituição da chamada à função system.  
    Ao executar o programa vai observar que a mensagem “Vou terminar” deixa de aparecer 
    no ecrã. A que é que se deve este comportamento? 
    Nota: pode comparar as várias funções da família exec escrevendo o seguinte comando 
    na shell: man 3 exec
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t r;

    printf("processo inicial: %d\n", getpid());

    r = fork();
    if (r == 0)
    {
        printf("\nPID = %d, PPID = %d\n\n", getpid(), getppid());

        exit(0);
    }

    sleep(3);
    execlp("ps", "ps", "-f", (char *)NULL);
  

    printf("\nVou terminar\n\n");
    execlp("ps", "ps", "-f", (char *)NULL);

    return (0);
}

/*
   
*/