#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void myhandler(int signum);

int main() {
	struct sigaction act;


	act.sa_handler=myhandler; //defenir a function handler com signal
	sigemptyset(&act.sa_mask); // limpar a mask do sinal
	act.sa_flags = 0;			//colocar a flag a 0;
	if(sigaction(SIGTERM, &act, NULL)==-1)
	{
		perror("sigaction");
		exit(1);
	};

	
	while(1){
		printf("Ola! Sou o processo %d. Mas pode confirmar com o comando ps.\n", getpid());
		sleep(10);
	}
  	     
	return 0;
}

void myhandler(int signum)
{
	printf("Recebi o sinal %d (%s). OK, vamos terminar\n", signum, strsignal(signum));
	exit(0);	
}

