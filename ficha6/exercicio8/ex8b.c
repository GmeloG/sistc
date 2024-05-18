#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_BUFFER 1024

int main(int argc, char *argv[])
{
  int sd, n;
  char buffer[MAX_BUFFER];

  // addrinfo struct to store the server address
  struct addrinfo hints, *a;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;

  if (argc != 3)
  {
    printf("Usage: %s <IP address> <port>\n", argv[0]);
    exit(1);
  }

  getaddrinfo(argv[1], argv[2], &hints, &a);
  sd = socket(AF_INET, SOCK_DGRAM, 0);

  while (1)
  {
    printf("Digite uma mensagem: ");
    fgets(buffer, MAX_BUFFER, stdin);
    sendto(sd, buffer, strlen(buffer), 0,
           a->ai_addr, a->ai_addrlen);
    n = read(sd, buffer, sizeof(buffer) - 1);
    buffer[n] = '\0';
    printf("Reply: %s", buffer);
  }

  return 0;
}