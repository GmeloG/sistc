#include "mycommon.h"
sem_t *psem;

int main(int argc, char **argv)
{
  //Adicionalmente, garanta que o acesso à memória partilhada é mutuamente exclusivo, seja entre 
  //ex4-reader e ex4-writer ou entre múltiplas execuções de ex4-writer. 
  //não sei se assim fica esclusivo
  psem = sem_open("/sem1", O_CREAT | O_RDWR, 0600, 1); // create se não existir cria se não não cria , read write, 0600 permissões 110 em, 1 valor inicial do semafaro.

  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1)
    handle_error("shm_open");

  int r = ftruncate(shm_fd, SHM_SIZE);
  if (r == -1)
    handle_error("ftruncate");

  char *shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED)
    handle_error("mmap");

  strcpy(shm_ptr, "");

  while (1)
  {
    sem_wait(psem);
    printf("Dados: %s\n", shm_ptr);
    sleep(2);
  }

  return 0;
}
