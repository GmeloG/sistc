#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <netdb.h>

#define MAX_MSG_SIZE 4096
#define serverName "ave.dee.isep.ipp.pt"
#define serverPort "80"
#define uriName "/sistc/ws/exemplo1/api/vehicles"
#define VEHIC_MAXPLEN 81
typedef struct
{
  char plate[6];
  char owner[VEHIC_MAXPLEN];
  double value;
} vehic_t;

void vehic_read(vehic_t *v);
void vehic_print(vehic_t *v);
int vehic_menu();
int parse_json(char c, vehic_t *v, char *buffer, int maxlen, int *nchars, int *state);
int my_connect(char *servername, char *port);
void submeter_dados(char *servername, char *uri, char *plate, char *owner, double value);
void recv_server_reply(int s);
void ReadFromServerVehicles(char *servername, char *uri, char *serverport);

int main(int argc, char *const argv[])
{
  int op;

  vehic_t v;

  while (1)
  {
    op = vehic_menu();
    switch (op)
    {
    case 0:
      exit(1);
    case 1: // insert new record

      vehic_read(&v);
      submeter_dados(serverName, uriName, v.plate, v.owner, v.value);

      break;
    case 2: // print all
      ReadFromServerVehicles(serverName, uriName, serverPort);

      // call vehic_print ( e.g., vehic_print(&v); ) for each record
      //...

      break;
    }
  }
  return 0;
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

int vehic_menu()
{
  int op;
  printf("\n0 - Exit\n");
  printf("1 - Insert new record\n");
  printf("2 - Print all\n");
  printf("\nEnter option: ");
  fflush(stdout);
  scanf("%d", &op);
  getchar(); // consume '\n'
  printf("\n");

  return op;
}

void vehic_read(vehic_t *v)
{
  char buf[VEHIC_MAXPLEN];

  printf("Plate (6 characters): ");
  fgets(buf, VEHIC_MAXPLEN, stdin);
  memcpy(v->plate, buf, 6);
  printf("Owner: ");
  fgets(v->owner, VEHIC_MAXPLEN, stdin);
  v->owner[strlen(v->owner) - 1] = 0; //"move" the end of the string to the place of '\n'
  printf("Value: ");
  scanf("%lf", &v->value);
  getchar(); // consume \n
}

void vehic_print(vehic_t *v)
{
  v->owner[VEHIC_MAXPLEN - 1] = 0; // double check
  // printf("%.6s - %s - %.2lf\n", v->plate, v->owner, v->value);
  printf("%.6s - %-50s - %14.2lf\n", v->plate, v->owner, v->value);
}

void submeter_dados(char *servername, char *uri, char *plate, char *owner, double value)
{
  char params[2048], msg[MAX_MSG_SIZE];

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
  // recv_server_reply(sd);
}

void recv_server_reply(int s)
{
  char buf[MAX_MSG_SIZE];

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

void recv_server_dados(int s, char *message)
{
  char buf[MAX_MSG_SIZE];

  FILE *file = fopen(s, "w+");
  if (file == NULL)
  {
    printf("Não foi possível abrir o arquivo.\n");
    return;
  }

  /*while (1)
  {
    int n = read(s, buf, 1);
    if (n <= 0)
      break;
    putchar(buf[0]);
    fflush(stdout);
  }
*/
  fprintf(file, "%s", message);
  fclose(file);
  return buf;
}

void ReadFromServerVehicles(char *servername, char *uri, char *serverport)
{
  char buffer[256];
  char *data = NULL;
  int nbytes, bytes_total = 0;
  char *line;

  int sd = my_connect(servername, serverport);
  sprintf(buffer, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", uri, servername);

  write(sd, buffer, strlen(buffer));

  while (1)
  { // ler todos os chunks
    // ler, para a variável “buffer”, linha com a indicação do número de bytes no próximo chunk
    read(sd, buffer, sizeof(buffer));

    char *line = strchr(buffer, "");
    while (line != NULL)
    {
      int foundEmptyLine;
      if (strcmp(line, "") == 0)
      {
        foundEmptyLine = 1;
      }
      else if (foundEmptyLine)
      {
        printf("Texto após a linha vazia: %s\n", line);
        foundEmptyLine = 0; // Reset the flag
      }
      line = strtok(NULL, "\n");
    }

    //printf("%s", line);

    // converte de texto/hexadecimal para formato nativo
    // sscanf(line, "%x", &nbytes);

    // se chegou ao último chunk (0 bytes), termina

    // data = realloc(data, bytes_total + nbytes + 1);
    //  ler bloco de dados de nbytes para a posição data+bytes_total
    // read(sd, data + bytes_total, nbytes);
    // bytes_total += nbytes;

    // ler o "\r\n" no final do chunk
    // read(sd, buffer, 2);
  }

  // Adiciona um caractere nulo no final para garantir que 'data' seja uma string válida
  data[bytes_total] = '\0';

  printf("Dados recebidos: %s\n", data);

  free(data);
}
