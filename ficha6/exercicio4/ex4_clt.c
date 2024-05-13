#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <ctype.h>

#define bufferSize 2000

// # function prototypes #//
int my_connect(char *servername, char *port);
void print_socket_address(int sd);

// # struct definitions #//
typedef struct
{
    char student_id[9];
    char text[2000]; // should be ‘\0’ terminated
} msg1_t;

typedef struct
{
    char text[2000];        // should be ‘\0’ terminated
    char student_name[100]; // should be ‘\0’ terminated
} msg2_t;

int main(int argc, char *const argv[])
{
    msg1_t msg1;
    msg2_t msg2;

    int nbytes;

    // check arguments is equal to 3 if not print usage
    if (argc != 3)
    {
        printf("usage: %s source\n", argv[0]);
        return 1;
    }
    // connect to server
    int socket_descriptor = my_connect(argv[1], argv[2]);

    // print local address
    print_socket_address(socket_descriptor);

    // Preenchimento e envio de msg1
    strcpy(msg1.student_id, "1211710\n");
    // Reading message from stdin
    do
    {
        printf("\n");
        printf("Enter message (max 2000 bytes):\n");
        fgets(msg1.text, sizeof(msg1.text), stdin);

        if (strlen(msg1.text) <= 1 || strchr(msg1.text, '\n') == NULL)
        {
            continue;
        }

        break;
    } while (1);

    printf("Mensagem enviada: %s", msg1.text);
    printf("Numero do estudante: %s", msg1.student_id);

    // Sending message to server
    if (write(socket_descriptor, &msg1, sizeof(msg1)) == -1)
    {
        perror("error write\n");
        close(socket_descriptor);
        exit(1);
    }

    // Recebimento e processamento de msg2
    if (read(socket_descriptor, &msg2, sizeof(msg2)) == -1)
    {
        perror("receive message:");
        close(socket_descriptor);
        exit(1);
    }
    printf("\n\nReceived message\n");
    printf("Mensagem recebida: %s", msg2.text);
    printf("Nome do estudante: %s", msg2.student_name);
    printf("Tamanho da mensagem e nome de estudante: %ld\n", (strlen(msg2.text)+strlen(msg2.student_name)));

    close(socket_descriptor);

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

void print_socket_address(int sd)
{
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    getsockname(sd, (struct sockaddr *)&addr, &addrlen);

    char hostname[256];
    char port[6];

    int n = getnameinfo((struct sockaddr *)&addr, addrlen, hostname, sizeof(hostname),
                        port, sizeof(port), NI_NUMERICHOST | NI_NUMERICSERV);
    if (n != 0)
        printf("%s\n", gai_strerror(n));
    else
        printf("Socket address: %s:%s\n", hostname, port);
}
