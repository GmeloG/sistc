/*
        O espaço de memória de cada processo é isolado dos restantes processos,
    independentemente do tipo de utilização da memória (variáveis globais, stack ou memória
    alocada dinamicamente na heap). Cada processo apresenta ao programa que está a
    executar um espaço de memória virtual e independente de todos os outros processos.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>

void *shmalloc(size_t size)
{
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
}

#define NELEM 20

int main(int argc, char *argv[])
{
    double *dados;
    pid_t r1, r2;

    dados = shmalloc(NELEM * sizeof(double));
    for (int i = 0; i < NELEM; ++i)
        dados[i] = i;

    r1 = fork();
    if (r1 == 0)
    {
        for (int i = 0; i < NELEM / 2; ++i)
            dados[i] = dados[i] * 2.0;
        exit(0);
    }

    r2 = fork();
    if (r2 == 0)
    {
        for (int i = NELEM / 2; i < NELEM; ++i)
            dados[i] = dados[i] * 2.0;
        exit(0);
    }

    waitpid(r1, NULL, 0);
    waitpid(r2, NULL, 0);

    for (int i = 0; i < NELEM; ++i)
        printf("%f ", dados[i]);
    printf("\n");

    return (0);
}

/*
        Como agora memoria dos vetores é partilha da pela função nmap o 
        vector dados contem a ultimas alteraçoes feitas.
    
       0.000000 2.000000 4.000000 6.000000 8.000000 10.000000 12.000000
       14.000000 16.000000 18.000000 20.000000 22.000000 24.000000 
       26.000000 28.000000 30.000000 32.000000 34.000000 36.000000 38.000000 

*/