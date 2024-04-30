#include "mycommon.h"
sem_t *psem;

int main(int argc, char **argv)
{
  //sem_unlink("/sem1"); // fechar o ficheiro do semaforo

  psem = sem_open("/sem1", O_CREAT | O_RDWR, 0666, 0); // create se não existir cria se não não cria , read write, 0600 permissões 110 em, 1 valor inicial do semafaro.

  char buffer[SHM_SIZE];

  int shm_fd = shm_open(SHM_NAME, O_RDWR, 0);
  if (shm_fd == -1)
    handle_error("shm_open");

  char *shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED)
    handle_error("mmap");

  printf(": ");
  fgets(buffer, SHM_SIZE, stdin);
  strcpy(shm_ptr, buffer);
  sem_post(psem);

  // sem_close("/sem1");
  sem_close(psem);

  return 0;
}
