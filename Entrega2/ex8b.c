#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_BUFFER 2048
#define PORT "3000"

int main(int argc, char *argv[])
{
  /* Adicionar esta linha no início da função main de cada programa */
  printf("1211710 - %s\n", __FILE__);
  int sd, n;
  char buffer[MAX_BUFFER];

  // addrinfo struct to store the server address
  struct addrinfo hints, *a;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;

  if (argc != 2)
  {
    printf("Usage: %s <IP address> \n", argv[0]);
    exit(1);
  }
  // getaddrinfo to get the server address
  getaddrinfo(argv[1], PORT, &hints, &a);
  sd = socket(AF_INET, SOCK_DGRAM, 0);

  while (1)
  {
    printf("Digite uma mensagem: ");
    fgets(buffer, MAX_BUFFER, stdin);
    sendto(sd, buffer, strlen(buffer), 0, a->ai_addr, a->ai_addrlen);
  }

  return 0;
}
