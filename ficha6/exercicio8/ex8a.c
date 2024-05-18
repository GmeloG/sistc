#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER 1024
#define PORT 3000

int main()
{
  int sockfd;
  struct sockaddr_in server_addr, client_addr;
  char buffer[MAX_BUFFER];
  socklen_t addr_size;

  // Criar socket UDP
  if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket");
    exit(1);
  }

  // Configurar detalhes do endereço
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Vincular socket ao endereço
  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
  {
    perror("bind");
    exit(1);
  }

  addr_size = sizeof(client_addr);

  // Receber mensagem
  int recv_len = recvfrom(sockfd, buffer, MAX_BUFFER, 0, (struct sockaddr *)&client_addr, &addr_size);
  if (recv_len > 0)
  {
    buffer[recv_len] = 0;
    printf("Mensagem recebida do cliente: %s\n", buffer);
    printf("Endereço IP do cliente: %s\n", inet_ntoa(client_addr.sin_addr));
    printf("Porto do cliente: %d\n", ntohs(client_addr.sin_port));
  }

  close(sockfd);
  return 0;
}