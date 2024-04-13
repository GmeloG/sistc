
/*
        Altere o programa de modo que a chamada à função fun2 seja feita num novo processo
    logo que a chamada a fun1 termina. Adicionalmente, o processo pai não deverá aguardar
    pela conclusão da execução de fun2 para executar novamente fun1.

*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void fun1(int *d)
{
    ++d[0];
    printf("Novo valor gerado em fun1 : % d\n", d[0]);
    sleep(2);
}
void fun2(int *d)
{
    sleep(1);
    printf("Valor processado por fun2 : % d\n", d[0]);
}
int main()
{
    /* linha de identificação */
    printf("1211710 - %s\n", __FILE__);

    int dados = 0;
    pid_t r;
    while (1)
    {
        // inicar fun1
        fun1(&dados);
        // criar novo processo filho
        r = fork();

        if (r == 0) // processo filho
        {
            // inicar fun2
            fun2(&dados);
            // fechar processo filho
            exit(0);
        }

        // processo pai não espera pelo filho
    }
    return 0;
}