#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

void wait_something();
void do_something();
void myHandler(int);

int main() {
  while(1) {
   wait_something();
   pid_t r = fork();
   if(r == 0) {
     do_something();
     return(0);
   }
  }
  return(0);
}

void wait_something() {
  sleep(1);
}

void do_something() {
  sleep(1);
}

void myHandler(int signum) {
  int wstatus;
  
  wait(&wstatus);
  
  if( WIFSIGNALED(wstatus) )
    printf("Child terminated by signal\n");
}
