#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>    /* For O_* constants */
#include <wait.h>

void myprint(const char *str);
void *myfunc(void *);

sem_t *psem;

int main()
{
  char *buf = (char *)malloc(256);

  // Usar semanforo sem nome em memoria compartilhada
  //como o semaforo sem nome não tem memoria partilhada entre processos, 
  //este tem de ser criado nos multiplos processos.
  psem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  sem_init(psem, 1, 1); 

  int r = fork();
  if (r == 0)
  {
    sprintf(buf, "%d: Ola -----------------------------------------\n", getpid());
    myfunc(buf);
  }
  else
  {
    sprintf(buf, "%d: Ole +++++++++++++++++++++++++++++++++++++++++\n", getpid());
    myfunc(buf);
    waitpid(r, NULL, 0);
  }

  sem_destroy(psem);
  munmap(psem, sizeof(sem_t));

  return (0);
}

void *myfunc(void *arg)
{
  while (1)
  {
    sleep(1);
    sem_wait(psem); 
    myprint((char *)arg);
    sem_post(psem);
  }
}


/*

      2 - Extraia o conteúdo do ficheiro ficha-sinc-ficheiros.zip para o seu diretório de trabalho.
    Analise o programa contido em ex2.c. A função myprint é a mesma função descrita no
    exercício 1 da ficha anterior. Pode utilizar o comando make para criar o executável ex2.
    2.1 - Utilize o mecanismo de semáforos com nome para garantir que a impressão de cada
    processo é enviada para o ecrã sem ser interrompida pelas impressões do outro processo.
    Observação: tenha atenção ao facto que o semáforo com nome mantém o seu estado mesmo
    após a terminação dos processos que o utilizam. Para repor o valor inicial do semáforo, terá que
    o apagar (sem_unlink).

    feito

    2.2 – Apresente um diagrama temporal da execução deste programa desde o seu arranque até
    ao momento em cada processo já executou pelo menos uma vez a função myprint.

    fazer em casa para estudar

    2.3 – Ao contrário do que se observou no exercício 1 da ficha anterior, basta agora fazer uma
    chamada à função malloc (nomeadamente, buf = malloc(256)) para que cada “tarefa”
    armazene e imprima a sua própria string. Porquê?

    Porque o processo filho tem um espaço de memória diferente do processo pai, logo o malloc aloca espaço de memória diferente para cada processo.

    2.4 (Memória partilhada sem nome) - Altere a linha
    char *buf = malloc(256);
    para
    char *buf = (char *) mmap(NULL, 256,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    e verifique que ambos os processos passam a imprimir a mesma mensagem.

    é impresso a mesma mensagem porque o espaço de memória é partilhado entre os dois processos.

    2.5 – Repita a alínea 2.1 usando um semáforo sem nome.




*/