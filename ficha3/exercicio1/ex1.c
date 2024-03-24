#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void sigusr1(int s)
{
    printf("Warning\n");
}
int main()
{
    sigset_t sigset;
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&(sa.sa_mask));
    sa.sa_handler = sigusr1;
    sigaction(SIGUSR1, &sa, NULL);  // manipulador para sigusr1
    signal(SIGTERM, SIG_IGN);  //imprime um warning na consola
    signal(SIGPIPE, SIG_IGN);       
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGPIPE);
    sigaddset(&sigset, SIGQUIT);
    sigprocmask(SIG_SETMASK, &sigset, NULL);
    while (1)
    {
        printf("Início\n");
        pause();
    }
}

/*

a) kill -QUIT 2000:
    O sinal QUIT é bloqueado pelo programa (usando sigprocmask)
    , então o programa não será afetado por este comando. 
    Mas se não existe o sigprocmask o programa era terminado

    b) kill -USR1 2000: 
    O programa definiu um manipulador para o sinal USR1 
    (usando sigaction), então quando este comando é executado, 
    o programa imprimirá “Warning” na consola.

    c) kill -USR2 2000: 
    O programa não definiu um manipulador para o sinal USR2,
    então o comportamento padrão será executado, que é terminar o programa.

    d) kill -PIPE 2000: 
    O sinal PIPE é ignorado pelo programa 
    (usando signal), 
    então o programa não será afetado por este comando.

    e) kill 2000: 
    Este comando envia o sinal TERM para o programa. 
    No entanto, o programa está configurado para ignorar o sinal TERM 
    (usando signal), então o programa não será afetado por este comando.

    f) kill -KILL 2000: 
    O sinal KILL não pode ser capturado ou ignorado, 
    então este comando terminará o programa imediatamente.

*/