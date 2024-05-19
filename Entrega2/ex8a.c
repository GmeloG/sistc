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

int main()
{
  /* Adicionar esta linha no início da função main de cada programa */
  printf("1211710 - %s\n", __FILE__);

  int socketDescriptor, n; // socket descriptor
  char buffer[MAX_BUFFER];

  // struct sockaddr_in server_addr para guardar o endereço do servidor
  struct addrinfo hints, *a;
  struct sockaddr src_addr;
  struct sockaddr_in client_addr;
  socklen_t src_len;

  // inicializar hints
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;  // IPv4
  getaddrinfo(NULL, PORT , &hints, &a); // getaddrinfo para obter o endereço do servidor

  // inicializar server_addr
  client_addr.sin_family = AF_INET; // IPv4
  client_addr.sin_port = htons(atoi(PORT)); // Porta do servidor
  client_addr.sin_addr.s_addr = inet_addr("Endereço IP do cliente"); // Endereço IP do cliente

  socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0); // criar socket UDP (SOCK_DGRAM) 
  bind(socketDescriptor, a->ai_addr, a->ai_addrlen); // associar o socket ao endereço do servidor
  printf("connected to a client\n");
  while (1)
  {
    printf("Waiting for message...\n");
    src_len = sizeof(client_addr); // tamanho do endereço do cliente
    n = recvfrom(socketDescriptor, buffer, sizeof(buffer) - 1, 0, 
                 &client_addr, &src_len); // receber mensagem do cliente
    buffer[n] = '\0'; // terminar a string

    printf("Received message: %s", buffer); // imprimir a mensagem recebida

    printf("Endereço IP do cliente: %s\n", inet_ntoa(client_addr.sin_addr));
    printf("Porto do cliente: %d\n", ntohs(client_addr.sin_port));
  }
}
