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
  int socketDescriptor, n;
  char buffer[MAX_BUFFER];
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;

  // Criar socket UDP
  if ((socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket");
    exit(1);
  }

  // Configurar detalhes do endereço
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Vincular socket ao endereço
  if (bind(socketDescriptor, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
  {
    perror("bind");
    exit(1);
  }

  printf("Aguardando mensagens no porto UDP %d...\n", PORT);

  while (1)
  {
    addr_size = sizeof(client_addr);
    n = recvfrom(socketDescriptor, buffer, MAX_BUFFER, 0, (struct sockaddr *)&client_addr, &addr_size);
    if (n > 0)
    {
      buffer[n] = '\0';
      printf("Mensagem recebida do cliente: %s\n", buffer);
      printf("Endereço IP do cliente: %s\n", inet_ntoa(client_addr.sin_addr));
      printf("Porto do cliente: %d\n", ntohs(client_addr.sin_port));
    }
  }

  close(socketDescriptor);
  return 0;
}
