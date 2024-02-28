/*
        Exercício 7
(funções, apontadores)
a) Analise as seguintes implementações3 das funções strcpy e memcpy da biblioteca
standard da linguagem C.
char *strcpy(char *dst, const char *src) {
char *cp = dst;
while (*cp++ = *src++);
return dst;
}
//equivalente à implementação acima
char *strcpy(char *dst, const char *src) {
char *cp = dst;
while(1) {
*cp = *src;
if(*cp == 0)
break;
cp++; src++;
}
return dst;
}
Qual a principal diferença entre ambos?
/* versão simplificada – normalmente copia
sizeof(int) bytes em cada iteração s
void *memcpy(void *dst, const void *src, size_t
n) {
char *s = (char *) src;
char *end = s + n;
char *d = (char *) dst;
while(s != end)
*d++ = *s++;
return dst;
}
b) Implemente um programa que copie para os vetores str2 e data2 o conteúdo dos
vetores str1 e data1, respetivamente, usando as funções mais adequadas da biblioteca
standard da linguagem C. O programa deverá também imprimir no ecrã a quantidade de
memória ocupada por cada variável (e.g., printf("%ld\n", sizeof(str1))).
Justifique os resultados.
char str1[] = "Teste!";
int data1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
char str2[1000];
int data2[1000];
c) Apresente uma possível implementação da função strcmp que retorne 0 em caso de
igualdade e -1 em todos os outros casos.
d) Apresente uma possível implementação da função memcmp que retorne 0 em caso de
igualdade e -1 em todos os outros casos.

*/

/*
    a) A função strcpy copia a string apontada no paramnetro src, até encontrar o \0 que é o fecho da string (if(*cp == 0)) a apontador dest diz qual é a varialvel que vai resever a string.
    the mencpy() function copies n bytes from the are src to area dest. precisa do tamanho the bytes que serão copiados.
    In summary, strcpy() is tailored for null-terminated strings, while memcpy() operates on memory and requires explicit size information. Remember that memcpy() does not stop at null terminators, which can lead to unexpected behavior if used with strings containing embedded null characters
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fuction that recieve 2 vectores of char and compares if that equal or not (0 equal, -1 not equal)
int StringCompareVariavels(char var1[], char var2[])
{

    if (strcmp(var1, var2) == 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int SizeCompareVariavels(int var1[], int var2[])
{

    if (sizeof(var1) == sizeof(var2))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int main()
{
    char str1[] = "Teste!";
    int data1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    char str2[1000];
    int data2[1000];

    char strcmp1[1000], strcmp2[1000];

    strcpy(str2, str1);
    memcpy(data2, data1, sizeof(data1));
    printf("copy!!");

    printf("Size of str1:       %ld \n", sizeof(str1));
    printf("Content of str1:     %s \n", str1);
    printf("Size of str2:       %ld \n", sizeof(str2));
    printf("Content of str2:     %s \n", str2);

    printf("Size of data1:      %ld \n", sizeof(data1));
    printf("Size of data2:      %ld \n", sizeof(data2));

    // part c compar strings
    printf("Write a string: ");
    scanf("%s", &strcmp1);

    printf("Write a string: ");
    scanf("%s", &strcmp2);

    // compare if string
    if (StringCompareVariavels(strcmp1, strcmp2) == 0)
    {
        printf("Equals Strings\n");
    }
    else
    {
        printf("Not equal strings\n");
    }

    /// parte d
    printf("Write a number: ");
    scanf("%d", &data1);

    printf("Write a number: ");
    scanf("%d", &data2);
    
    if (SizeCompareVariavels(strcmp1, strcmp2) == 0)
    {
        printf("Equals int\n");
    }
    else
    {
        printf("Not equal int\n");
    }

    return 0;
}