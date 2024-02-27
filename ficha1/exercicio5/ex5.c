/*
    Exercício 5  (leitura de valores, ciclos, condições, funções, passagem por valor vs passagem por referência)

    a) Escreva um programa que leia dois valores numéricos (inteiros) a partir do teclado e
    que imprima o resultado da divisão inteira do primeiro valor (dividendo) pelo segundo
    (divisor) assim como o respetivo resto (use o operador %). O programa deverá repetir o
    procedimento (ler valores e executar divisão) até que o valor introduzido para o dividendo
    seja 0.
    Para ler dados introduzidos pelo utilizador pode usar a já bem conhecida função scanf2:
    int scanf (const char *template, ...)
    Exemplo:
    int a;
    scanf("%d", &a);
    Crie o respetivo executável e teste o seu programa.
    b) Altere o seu programa de forma a que os valores do quociente e do resto sejam
    calculados numa nova função (minha_divisao, descrita abaixo) e guardados em
    variáveis declaradas na função main. Para tal, pretende-se que use a passagem de
    parâmetros por referência. A função deverá obedecer ao seguinte protótipo:
    void minha_divisao(int dividendo, int divisor, int *ptr_quociente, int
    *ptr_resto);
    A impressão do quociente e do resto deverá continuar a ser feita na função main.

*/

#include <stdio.h>
#include <stdlib.h>

void parteA()
{
    int dividendo = 1, divisor, quociente, resto;

    while (dividendo != 0)
    {
        printf("\nDigite um dividendo inteiro: ");
        scanf("%d", &dividendo);
        if (dividendo != 0)
        {
            printf("Digite um divisor inteiro: ");
            scanf("%d", &divisor);

            quociente =  dividendo / divisor;
            resto = dividendo % divisor;

            printf("\nQuociente = %d", quociente);
            printf("\nResto = %d \n", resto);
        }
    }
}

void minha_divisao(int dividendo, int divisor, int *ptr_quociente, int *ptr_resto)
{

    *ptr_quociente = dividendo / divisor ;
    *ptr_resto = dividendo % divisor ;

}
void parteB()
{
    int dividendo = 1, divisor, quociente, resto;

    while (dividendo != 0)
    {
        printf("\nDigite um dividendo inteiro: ");
        scanf("%d", &dividendo);
        if (dividendo != 0)
        {
            printf("Digite um divisor inteiro: ");
            scanf("%d", &divisor);

            minha_divisao(dividendo, divisor, &quociente, &resto);

            printf("\nQuociente = %d", quociente);
            printf("\nResto = %d \n", resto);
        }
    }
}

int main()
{

    // parteA();
    parteB();

    return 0;
}