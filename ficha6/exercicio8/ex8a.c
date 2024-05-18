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

  int socketDescriptor, n; // socket descriptor
  char buffer[MAX_BUFFER];

  // struct sockaddr_in server_addr para guardar o endereço do servidor
  struct addrinfo hints, *a;
  struct sockaddr src_addr;
  socklen_t src_len;

  if (argc != 2)
  {
    printf("Usar: %s <IP address>\n", argv[0]);
    exit(1);
  }
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  getaddrinfo(NULL, argv[1], &hints, &a);

  socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
  bind(socketDescriptor, a->ai_addr, a->ai_addrlen);
  printf("connected to a client\n");
  while (1)
  {
    printf("Waiting for message...\n");
    src_len = sizeof(src_addr);
    n = recvfrom(socketDescriptor, buffer, sizeof(buffer) - 1, 0,
                 &src_addr, &src_len);
    buffer[n] = '\0';
    printf("Received message: %s", buffer);
    sendto(socketDescriptor, buffer, n, 0, &src_addr, src_len);
  }
}