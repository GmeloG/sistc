/*
        Exercício 5
    Escreva um programa que execute a seguinte sequência de ações:
    ▪ Alocar um bloco de 8 bytes da memória de dados (char *ptr = malloc(8)).
    ▪ Guardar a string “pai” no bloco alocado (use a função strcpy).
    ▪ Criar um processo filho que deverá escrever a string “filho” no bloco apontado
    por ptr, fazer a impressão do conteúdo do mesmo no ecrã e terminar.
    ▪ Aguardar a conclusão do processo filho (waitpid) e imprimir a string apontada
    por ptr.
    Qual o resultado esperado?

    Processo filho:     filho
    Processo pai:       pai

    O que foi impresso em ultimo foi o valor "pai" visto que quando criamos um processo
    filho toda a memoria é copiada para outro processor e não pode ser acedia a menos
    que fazermos um mmap as memorias.

*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    /* linha de identificação */
    printf("1211710 - %s\n", __FILE__);

    // Declare a pointer to a character array of size 8
    char *ptr = malloc(8);

    // Assign the address of the pointer to the string "pai"
    ptr = strcpy(ptr, "pai");

    // Fork a child process
    pid_t r = fork();

    // if is on a child process
    if (r == 0)
    {
        // Assign the address of the pointer to the string "filho"
        ptr = "filho";

        // Print a message indicating that the child process is writing to the memory block
        printf("Processo filho:     %s  \n", ptr);
        exit(0);
    }

    // Wait for the child process to finish
    waitpid(r, NULL, 0);

    // Print the contents of the memory block
    printf("Processo pai:       %s  \n", ptr);

    // Clean up malloc
    free(ptr);

    return 0;
}