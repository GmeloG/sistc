#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>


int main()
{
    //criar um vetor de inteiros com um elemento e inicializa-lo 0. O bloco de memória é partilhado e anónimo.
    int *v = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    //criar um semáforo com o nome "/sem1" e inicializa-lo a 0
    sem_t *psem = sem_open("/sem1", O_CREAT | O_RDWR, 0600, 0);

    v[0] = 0;
    int r, n = 0;
    //criar um processo filho
    
    r = fork();
    ++n;  
    ++v[0];
    if (r == 0)
    {
        sem_wait(psem); //Esta linha bloqueia o semáforo, até que o processo pai o liberte.
        printf("%d: *v = %d, n = %d\n", getpid(), v[0], n);
        return (0);
    }
    sleep(5);
    printf("%d: *v = %d, n = %d\n", getpid(), v[0], n);
    sem_post(psem); //Esta linha liberta o semáforo, permitindo que o processo filho prossiga.
    
    sem_close("/sem1");
    
    return (0);
}
//47569: *v = 2, n = 1
//47570: *v = 2, n = 1