#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int i = 0;      //variavel global
int main()
{
    pid_t r = fork();

    //processo filho
    if (r == 0)
    {
        sleep(10);
        printf("%d: %d %d\n", getpid(), i, r);  //get pid processo do filho, 0 ,0 
        return 0;                               //desbloqueia o wait(NULL)
    }

    //processo pai
    i = i + 1;
    wait(NULL);         //espera que um processo filho acabe
    printf("%d: %d %d\n", getpid(), i, r);      //get pid processo do pai, r tem o valor do pid do filho 
    return 0;
}


/*
        Exercicio de exame
        a) Indique a sucessão de mensagens impressas no ecrã durante a execução do programa. 
        
        3399: 0 0
        3398: 1 3399
        
        b) Por que motivo a impressão da linha 8 é feita antes da impressão da linha 14 apesar 
        da pausa de 10 segundos da linha 7? 
        
        wait(NULL);
        quando fazes wait(NULL) o processo que esta a ser executado esta função fica a espera até algum processo termine.
        
        
        c) Indique a sucessão de mensagens no caso da linha 9 ser omitida. 
        14039: 0 0
        14039: 1 0
        14038: 1 14039
*/