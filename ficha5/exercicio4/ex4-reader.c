#include "mycommon.h"


int main(int argc, char** argv) {
  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1) handle_error("shm_open");

  int r = ftruncate(shm_fd, SHM_SIZE);
  if (r == -1) handle_error("ftruncate");

  char* shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED) handle_error("mmap");

  strcpy(shm_ptr, "");
  
  while (1) {
    printf("Dados: %s\n", shm_ptr);
    sleep(2);
  }
   
  return 0;
}
