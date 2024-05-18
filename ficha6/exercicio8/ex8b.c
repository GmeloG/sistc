#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER 1024
#define PORT 3000

int main(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in server_addr;
  char buffer[MAX_BUFFER];

  if (argc != 2)
  {
    fprintf(stderr, "uso: %s <IP address>\n", argv[0]);
    exit(1);
  }

  // Criar socket UDP
  if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket");
    exit(1);
  }

  // Configurar detalhes do endereço
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  while (1)
  {
    printf("Digite uma mensagem: ");
    fgets(buffer, MAX_BUFFER, stdin);

    // Enviar mensagem
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
  }

  close(sockfd);
  return 0;
}