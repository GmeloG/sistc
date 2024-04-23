#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void myprint(const char *str);
void *myfunc(void *);

pthread_mutex_t mutex;

int main()
{
  pthread_t thread1, thread2;
  char *buf1 = (char *)malloc(256);
  char *buf2 = (char *)malloc(256);

  // Inicializar o mutex
  pthread_mutex_init(&mutex, NULL);

  sprintf(buf1, "%d: Ola -----------------------------------------\n", getpid());
  sprintf(buf2, "%d: Ole +++++++++++++++++++++++++++++++++++++++++\n", getpid());

  pthread_create(&thread1, NULL, myfunc, buf1);
  pthread_create(&thread2, NULL, myfunc, buf2);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  pthread_mutex_destroy(&mutex);

  return (0);
}

void *myfunc(void *arg)
{
  while (1)
  {
    sleep(1);
    pthread_mutex_lock(&mutex); 
    myprint((char *)arg);
    pthread_mutex_unlock(&mutex);
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