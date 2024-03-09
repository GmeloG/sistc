/*
    Quando um processo passa ao estado zombie deixa de usar tempo de CPU e a maior parte
da memória que utilizava é libertada. No entanto, o seu identificador de processo continua
atribuído. Tendo em conta esse aspeto, a acumulação de processos zombie pode, a certa
atura, impedir a criação de novos processos, devido à ocupação de todos os
identificadores de processo.
Uma das formas de evitar acumulação de processos zombie sem ser necessário que o
respetivo pai fique a aguardar ou tenha de detetar a sua terminação é criar um processo
“neto” para realizar a tarefa que queremo
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
        printf("Novo processo, que irá terminar imediatamente: %d\n", getpid());
        r = fork();
        if (r != 0)
            exit(0);

        sleep(1);
        printf("\nPID = %d, PPID = %d\n\n", getpid(), getppid());
        sleep(3);
        printf("\nPID = %d, PPID = %d vai terminar\n\n", getpid(), getppid());
        exit(0);
    }

    sleep(2);
    system("ps -f");

    printf("\nChamada à função wait\n\n");

    wait(NULL);
    system("ps -f");
    sleep(2);
    system("ps -f");

    return (0);
}

/*
        Observe que, tal como desejado, o processo criado pela segunda chamada à função fork
    não fica zombie. O processo criado pelo primeiro fork só fica zombie até ser chamada a
    função waitpid, o que normalmente é feito imediatamente (as chamadas à função sleep
    só foram acrescentadas para se perceber melhor o funcionamento do programa).

    processo inicial: 26706
    Novo processo, que irá terminar imediatamente: 26707

    PID = 26708, PPID = 429

    UID          PID    PPID  C STIME TTY          TIME CMD
    melo         626     520  0 18:11 pts/5    00:00:00 /bin/bash --init-file /home/melo/.vscode-server/bin/019f4d1419fbc8219a181fab7892ebccf7ee29a2/out/vs/workbench/contrib/terminal/browser/media/shellIntegration-bash.
    melo       26706     626  0 19:13 pts/5    00:00:00 ./ex8
    melo       26707   26706  0 19:13 pts/5    00:00:00 [ex8] <defunct>
    melo       26708     429  0 19:13 pts/5    00:00:00 ./ex8
    melo       26731   26706  0 19:13 pts/5    00:00:00 sh -c ps -f
    melo       26732   26731  0 19:13 pts/5    00:00:00 ps -f

    Chamada à função wait

    UID          PID    PPID  C STIME TTY          TIME CMD
    melo         626     520  0 18:11 pts/5    00:00:00 /bin/bash --init-file /home/melo/.vscode-server/bin/019f4d1419fbc8219a181fab7892ebccf7ee29a2/out/vs/workbench/contrib/terminal/browser/media/shellIntegration-bash.
    melo       26706     626  0 19:13 pts/5    00:00:00 ./ex8
    melo       26708     429  0 19:13 pts/5    00:00:00 ./ex8
    melo       26733   26706  0 19:13 pts/5    00:00:00 sh -c ps -f
    melo       26734   26733  0 19:13 pts/5    00:00:00 ps -f

    PID = 26708, PPID = 429 vai terminar

    UID          PID    PPID  C STIME TTY          TIME CMD
    melo         626     520  0 18:11 pts/5    00:00:00 /bin/bash --init-file /home/melo/.vscode-server/bin/019f4d1419fbc8219a181fab7892ebccf7ee29a2/out/vs/workbench/contrib/terminal/browser/media/shellIntegration-bash.
    melo       26706     626  0 19:13 pts/5    00:00:00 ./ex8
    melo       26752   26706  0 19:13 pts/5    00:00:00 sh -c ps -f
    melo       26753   26752  0 19:13 pts/5    00:00:00 ps -f

*/
