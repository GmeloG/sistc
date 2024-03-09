/*

        Escreva um programa que crie um novo processo. O processo filho deverá imprimir os
    seus PID e PPID e terminar. O processo pai deverá aguardar alguns instantes (e.g., 3
    segundos, usando função sleep) e então executar o comando “ps -f” através da função
    system:
    int system(const char *command); //da norma ANSI C
    Antes de terminar, o processo pai deverá imprimir a mensagem “Vou terminar”. Após a
    terminação do programa volte a executar o comando “ps -f”, na shell. Analise os
    resultados.

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
    system("ps -f");

    printf("\nVou terminar\n\n");
    system("ps -f");

    return (0);
}

/*
    processo inicial: 31211

    PID = 31212, PPID = 31211

    UID          PID    PPID  C STIME TTY          TIME CMD
    melo         626     520  0 18:11 pts/5    00:00:00 /bin/bash --init-file /home/melo/.vscode-server/bin/019f4d1419fbc8219a181fab7892ebccf7ee29a2/out/vs/workbench/contrib/terminal/browser/media/shellIntegration-bash.
    melo       31211     626  0 19:22 pts/5    00:00:00 ./ex9
    melo       31212   31211  0 19:22 pts/5    00:00:00 [ex9] <defunct>
    melo       31248   31211  0 19:22 pts/5    00:00:00 sh -c ps -f
    melo       31249   31248  0 19:22 pts/5    00:00:00 ps -f

    Vou terminar

    UID          PID    PPID  C STIME TTY          TIME CMD
    melo         626     520  0 18:11 pts/5    00:00:00 /bin/bash --init-file /home/melo/.vscode-server/bin/019f4d1419fbc8219a181fab7892ebccf7ee29a2/out/vs/workbench/contrib/terminal/browser/media/shellIntegration-bash.
    melo       31211     626  0 19:22 pts/5    00:00:00 ./ex9
    melo       31212   31211  0 19:22 pts/5    00:00:00 [ex9] <defunct>
    melo       31250   31211  0 19:22 pts/5    00:00:00 sh -c ps -f
    melo       31251   31250  0 19:22 pts/5    00:00:00 ps -f
*/