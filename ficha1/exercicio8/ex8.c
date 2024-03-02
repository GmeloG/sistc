/*
        Exercício 8 (struct, typedef)

    Pretende-se escrever um programa que permita fazer a leitura de campos relativos ao
registo de automóveis e armazená-los numa variável. Os dados a guardar para cada
automóvel são a matrícula (vetor de 6 caracteres, não terminado com '\0'), o nome do
proprietário, até um máximo de 80 caracteres (string, cadeia de caracteres terminada com
'\0') e o valor comercial do automóvel.

    Cada registo deverá obedecer à seguinte estrutura:
    #define VEHIC_MAXPLEN 81
typedef struct {
char plate[6];
char owner[VEHIC_MAXPLEN];
double value;
} vehic_t;

8.1) Implemente uma função void vehic_print(vehic_t *v) que imprima no ecrã
os campos da estrutura apontada por v. O campo value deverá ser impresso apenas com
duas casas decimais ("%.2lf" na formatação do printf). A função deverá ser testada
com o seguinte programa:

int main(){
vehic_t v1 = {{'1', '4', '7', '9', 'H', 'P'},
"Carlos Reis Salvador Almada",
2999999.99};
vehic_print(&v1);
return 0;
}


8.2 – Implemente uma função void vehic_read(vehic_t *v) que solicite os dados
do veículo ao utilizador (nome do proprietário, matrícula e valor comercial) e preencha a
estrutura apontada por v com os dados obtidos do teclado. A função deverá ser testada
com o seguinte programa:

int main() {
vehic_t v1;
vehic_read(&v1);
vehic_print(&v1);
return 0;
}

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEHIC_MAXPLEN 81

typedef struct
{
    char plate[6];
    char owner[VEHIC_MAXPLEN];
    double value;
} vehic_t;

// função escrever a estrutura de veiculos
void vehic_print(vehic_t *v)
{

    printf("Plate: %.6s \n", v->plate); // usado o %.6s porque string não termina em '\0'
    printf("Owner: %s \n", v->owner);
    printf("Value: %.2lf \n", v->value);
}

void vehic_read(vehic_t *v)
{
    
    char tempPlate[50];
    char tempValue[30];

    // read and store in struct plate number
    printf("Plate:");
    gets(tempPlate);
    memcpy(v->owner,tempPlate,6);

    //read and store owner code
    printf("Owner:");
    fgets(&v->owner,81,stdin);

    //read and store value of struct
    printf("Value: ");
    fgets(tempValue,15,stdin);
    v->value = atof(tempValue);
}

int main()
{
    /*
    //8.1
    vehic_t v1 = {{'1', '4', '7', '9', 'H', 'P'},
    "Carlos Reis Salvador Almada",2999999.99};

    vehic_print(&v1);
    */

    // 8.2

    vehic_t v1;
    vehic_read(&v1);
    vehic_print(&v1);

    return 0;
}
