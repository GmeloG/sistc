/*
        Exercício 6 (Processos “orfão”)
    Quando um processo termina, os seus processos filho continuam em execução. No
    entanto, o sistema atribui um novo processo pai a esses processos.
    De forma a verificar o funcionamento deste mecanismo, faça as seguintes alterações ao
    código do Exercício 1:
    • Na linha 18, altere o argumento da função sleep para 2 (pausa de 2 segundos).
    • Na linha 26, altere o argumento da função sleep para 1 (pausa de 1 segundo).
    Após realizar as alterações, compile e teste o programa. Vai observar que o processo
    inicial termina ao fim de aproximadamente 10 segundos, ficando a linha de comando
    novamente disponível para execução de comandos, enquanto o seu processo filho
    continua as suas impressões. Deverá também observar que, após a terminação do processo
    inicial, o valor retornado pela função getppid no seu processo filho muda.
    Use o comando ps -e para identificar qual o programa correspondente a esse PID

*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{

    pid_t r;

    printf("\nProcesso %d a executar %s\n\n", getpid(), argv[0]);

    r = fork(); // retorna 0 no novo processo

    printf("Valor de retorno da função fork para o processo %d: %d\n\n", getpid(), r);

    if (r == 0)
    {
        for (int i = 0; i < 10; ++i)
        {
            sleep(2);
            printf("Novo processo, com PID = %d (\"filho\" de %d)\n",
                   getpid(), getppid());
        }
        exit(0); // termina o novo processo
    }

    for (int i = 0; i < 10; ++i)
    {
        sleep(1);
        printf("Processo inicial (PID = %d, \"filho\" de %d).\n",
               getpid(), getppid());
    }

    return (0);
}

/*
    Processo 9303 a executar ./ex6

    Valor de retorno da função fork para o processo 9303: 9304

    Valor de retorno da função fork para o processo 9304: 0

    Processo inicial (PID = 9303, "filho" de 657).
    Novo processo, com PID = 9304 ("filho" de 9303)
    Processo inicial (PID = 9303, "filho" de 657).
    Processo inicial (PID = 9303, "filho" de 657).
    Novo processo, com PID = 9304 ("filho" de 9303)
    Processo inicial (PID = 9303, "filho" de 657).
    Processo inicial (PID = 9303, "filho" de 657).
    Novo processo, com PID = 9304 ("filho" de 9303)
    Processo inicial (PID = 9303, "filho" de 657).
    Processo inicial (PID = 9303, "filho" de 657).
    Novo processo, com PID = 9304 ("filho" de 9303)
    Processo inicial (PID = 9303, "filho" de 657).
    Processo inicial (PID = 9303, "filho" de 657).
    Novo processo, com PID = 9304 ("filho" de 9303)
    Processo inicial (PID = 9303, "filho" de 657).
    melo@SurfaceMelo:~/isep/sistc/ficha2/exercicio6$ 
    Novo processo, com PID = 9304 ("filho" de 449)
    Novo processo, com PID = 9304 ("filho" de 449)
    Novo processo, com PID = 9304 ("filho" de 449)
    Novo processo, com PID = 9304 ("filho" de 449)
    Novo processo, com PID = 9304 ("filho" de 449)
    ps -e
        PID TTY          TIME CMD
        
        449 ?        00:00:00 Relay(450)



        Neste programa quando o processo pai acabou foi criado um novo processo pai para 
        poder correr um novo processo filho.

*/