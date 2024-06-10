#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MSG_SIZE 4096
#define serverName "ave.dee.isep.ipp.pt"
#define serverPort "80"
#define uriName "/sistc/ws/exemplo1/api/vehicles"

int my_connect(char *servername, char *port);
void recv_server_reply(int s);
void submeter_dados(char *servername, char *uri, char *plate, char *owner, double value);
void input_vehicle_data(Vehicle *vehicle);

// Estrutura de dados para veiculo
typedef struct
{
  char plate[7];
  char owner[100];
  double value;
} Vehicle;

int main(int argc, char *const argv[])
{
  Vehicle v;
  
  input_vehicle_data(&v);

  submeter_dados(serverName, uriName, v.plate, v.owner, v.value);

  return 0;
}
// Função para inserir dados do veiculo
void input_vehicle_data(Vehicle *vehicle)
{
  // Data input
  printf("Inserir dados do veiculo\n");

  //matricula com 6 caracteres
  printf("Matricula: ");
  fgets(vehicle->plate, sizeof(vehicle->plate), stdin);
  printf("Matricula: %s\n", vehicle->plate);

  //proprietario com 100 caracteres
  printf("Proprietario: ");
  fgets(vehicle->owner, sizeof(vehicle->owner), stdin);

  // Limpa o buffer de entrada
  int c;
  while ((c = getchar()) != '\n' && c != EOF);

  printf("Valor: ");
  scanf("%lf", &(vehicle->value));
}
// Função para submeter dados
void submeter_dados(char *servername, char *uri, char *plate, char *owner, double value)
{
  char params[4096], msg[8192];

  int sd = my_connect(servername, "80");

  // construir mensagem HTTP POST
  sprintf(params,
          "{\"plate\":\"%.6s\",\"owner\":\"%s\",\"value\":\"%.2lf\"}",
          plate, owner, value);

  sprintf(msg,
          "POST %s HTTP/1.1\r\n"
          "Host: %s\r\n"
          "Content-Type: application/json\r\n"
          "Content-Length: %ld\r\n"
          "Connection: close\r\n"
          "\r\n"
          "%s",
          uri, servername, strlen(params), params);
  // enviar pedido HTTP POST
  write(sd, msg, strlen(msg));

  // receber e imprimir resposta do servidor web
  //recv_server_reply(sd);
}

void recv_server_reply(int s)
{
  char buf[4096];

  printf("Reply from server: ");
  while (1)
  {
    int n = read(s, buf, 1);
    if (n <= 0)
      break;
    putchar(buf[0]);
    fflush(stdout);
    if (buf[0] == '\n')
      printf(": ");
  }

  printf("\n\n");
}

int my_connect(char *servername, char *port)
{

  // get server address using getaddrinfo
  struct addrinfo hints;
  struct addrinfo *addrs;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  int r = getaddrinfo(servername, port, &hints, &addrs);
  if (r != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
    exit(1);
  }

  // create socket
  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1)
  {
    perror("socket");
    exit(2);
  }

  // connect to server
  r = connect(s, addrs->ai_addr, addrs->ai_addrlen);
  if (r == -1)
  {
    perror("connect");
    close(s);
    exit(3);
  }

  freeaddrinfo(addrs);
  return s;
}
