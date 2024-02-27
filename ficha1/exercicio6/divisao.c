/*
        Exercício 6(cadeias de caracteres, parâmetros da função main)
    Modifique o programa do exercício 5b) de forma a que quando forem indicados dois
    valores na linha de comando (por exemplo, “./divisao 10 2”) o programa execute a
    função minha_divisao (exercício 5b), usando os valores indicados na linha de
    comando, e termine a sua execução. Nos restantes casos, o programa deverá comportar-
    se como descrito no exercício 5b).
    Para tal, a função main deverá ter os argumentos definidos tal como indicado abaixo:
    int main(int argc, char* argv[])
    Para converter uma string para um inteiro pode usar a função atoi.
    Crie o respetivo executável e teste o seu programa
*/

#include <stdio.h>
#include <stdlib.h>

void minha_divisao(int dividendo, int divisor, int *ptr_quociente, int *ptr_resto)
{

    *ptr_quociente = dividendo / divisor;
    *ptr_resto = dividendo % divisor;
}

int main(int argc, char *argv[])
{

    int quociente, resto;
    if (argc == 3)
    {

        minha_divisao(atoi(argv[1]), atoi(argv[2]), &quociente, &resto);

        printf("\nQuociente = %d", quociente);
        printf("\nResto = %d \n", resto);
    }
    else
    {
        printf("Função mal intruduzida \n ./divisao (dividendo) (divisor)\n");
    }
}