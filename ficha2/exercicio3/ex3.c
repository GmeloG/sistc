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

#define NELEM 20

int main(int argc, char *argv[])
{
    double *dados;
    pid_t r1, r2;

    dados = malloc(NELEM * sizeof(double));
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
        Na linha 9, é armazenada em dados uma sequência de valores correspondente à 
    progressão aritmética 0, 1, … , 19. De seguida todos os valores são multiplicados por 2, 
    sendo que, cada metade dos elementos é processada num diferente processo (linhas 13, 
    14, 20 e 21). 

    
        Qual a sequência de valores impressa por este programa (linhas 28 e 29)? Por que motivo 
    o vetor dados mantém os valores iniciais? 

    0.000000 1.000000 2.000000 3.000000 4.000000 5.000000 6.000000 7.000000 8.000000 
    9.000000 10.000000 11.000000 12.000000 13.000000 14.000000 15.000000 16.000000 17.000000 
    18.000000 19.000000 

        No primeiro processo filho a primeira metade do vetor é multiplicada por 2, e no segundo processo 
    filho a segunda metade do vetor é multiplicada por 2. Depois ao fazer o exit o vetores dos processo 
    filhos são eliminados. E o program imprime o vector do processo pai que não foi alterado.  

*/