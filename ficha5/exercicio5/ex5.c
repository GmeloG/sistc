#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>    /* For O_* constants */
#include <wait.h>


int main()
{
    int *v = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sem_t *psem = sem_open("/sem1", O_CREAT | O_RDWR, 0600, 1);

    v[0] = 0;
    int r, n = 0;
    for (int i = 0; i < 2; ++i)
    {
        sleep(1);
        r = fork();
        ++n;
        if (r == 0)
        {
            r = fork();
            ++v[0];
            if (r == 0)
            {
                sem_wait(psem);
                sleep(3);
                sem_post(psem);
                printf("n = %d, v = %d, pid = %d, ppid = %d.\n", n, v[0], getpid(), getppid());
                return (0);
            }
            sleep(1);
            printf("%d a terminar; *v = %d.\n", getpid(), v[0]);
            exit(0);
        }
        waitpid(r, NULL, 0);
        printf("%d terminado; *v = %d, n = %d.\n", r, v[0], n);
    }
    return (0);
}


/*
            
    Apresente a sequência de impressões produzidas por este programa. Assuma que não existem 
    interferências de outros processos no sistema, que o identificador do processo inicial é 2000 e 
    que o(s) novo(s) processo(s) toma(m) o(s) valor(es) seguinte(s). Assuma que que a instrução 
    ++v[0] é atómica. Apresente um diagrama temporal representativo da execução do programa 
    e justifique sucintamente. A sequência de impressões deve ser apresentada de forma destacada. 

    2733 a terminar; *v = 2.
    2733 terminado; *v = 2, n = 1.
    n = 1, v = 4, pid = 2734, ppid = 443.
    2766 a terminar; *v = 4.
    2766 terminado; *v = 4, n = 2.


*/