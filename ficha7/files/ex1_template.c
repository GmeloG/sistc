#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int my_connect(char *servername, char *port);
void recv_server_reply(int s);

char http_example_msg[] = 
  "POST /sistc/ws/exemplo1/api/vehicles HTTP/1.1\r\n"
  "Host: ave.dee.isep.ipp.pt\r\n"
  "Content-Type: application/json\r\n"
  "Content-Length: 58\r\n"
  "Connection: close\r\n"
  "\r\n"
  "{\"plate\":\"11AA22\",\"owner\":\"John Jones\",\"value\":\"10000.00\"}";

int main (int argc, char* const argv[]) {

  int sd = my_connect("ave.dee.isep.ipp.pt", "80");
  
  //enviar dados
  //... (completar)
  write(sd, http_example_msg, strlen(http_example_msg));
  
  //receber e imprimir resposta do servidor web
  recv_server_reply(sd);
  
  return 0;
}


void recv_server_reply(int s) 
{
  char buf[4096];

  printf("Reply from server: ");
  while(1) 
  {
    int n = read(s, buf, 1);
    if(n <= 0)
      break;
    putchar(buf[0]);
    fflush(stdout);
    if(buf[0]=='\n')
      printf(": ");
  }
  

  printf("\n\n");
}


int my_connect(char *servername, char *port) {

  //get server address using getaddrinfo
  struct addrinfo hints;
  struct addrinfo *addrs;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  int r = getaddrinfo(servername, port, &hints, &addrs);
  if (r != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
    exit(1);
  }
  
  //create socket
  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1) {
    perror("socket");  
    exit(2);
  }
  
  //connect to server
  r = connect(s, addrs->ai_addr, addrs->ai_addrlen);
  if (r == -1) {
    perror("connect");  
    close(s);
    exit(3);
  }
  
  freeaddrinfo(addrs);
  return s;
}


