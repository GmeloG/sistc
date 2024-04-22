#include "mycommon.h"


int main(int argc, char** argv){
  char buffer[SHM_SIZE];
  
  int shm_fd = shm_open(SHM_NAME, O_RDWR, 0);
  if (shm_fd == -1) handle_error("shm_open");

  char* shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED) handle_error("mmap");

  printf(": ");
  fgets(buffer, SHM_SIZE, stdin);
  strcpy(shm_ptr, buffer);
  
  return 0;
}
