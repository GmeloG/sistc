#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// # function prototypes #//
int my_create_server_socket(char *port);
void print_address(const struct sockaddr *clt_addr, socklen_t addrlen);
int myReadLine(int s, char *buf, int count);
void upperCase(char *str2,char *str1);

// # struct definitions #//
typedef struct
{
    char student_id[7];
    char text[2000]; // should be ‘\0’ terminated
} msg1_t;

typedef struct
{
    char text[2000];        // should be ‘\0’ terminated
    char student_name[100]; // should be ‘\0’ terminated
} msg2_t;

int main(int argc, char *argv[])
{

    struct sockaddr clt_addr;
    socklen_t addrlen;

    msg1_t msg1;
    msg2_t msg2;

    int new_socket_descriptor, socket_descriptor; // socfd socket file descriptor
    int nbytes;

    if (argc != 2)
    {
        printf("Usage: %s port_number\n", argv[0]);
        exit(1);
    }

    signal(SIGPIPE, SIG_IGN); // ignore SIGPIPE

    socket_descriptor = my_create_server_socket(argv[1]);

    while (1)
    {
        printf("Waiting connection\n");

        addrlen = sizeof(clt_addr);
        new_socket_descriptor = accept(socket_descriptor, &clt_addr, &addrlen);
        if (new_socket_descriptor < 0)
        {
            perror("accept");
            sleep(1);
            continue;
        }
        print_address(&clt_addr, addrlen);

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }

        if (pid == 0)
        {

            nbytes = recv(new_socket_descriptor, &msg1, sizeof(msg1), 0);
            if (nbytes == -1)
            {
                perror("receive message:");
                close(socket_descriptor);
                exit(1);
            }
            printf("Mensagem recebida: %s\n", msg1.text);
            printf("Numero do estudante: %s\n", msg1.student_id);

            upperCase(msg2.text,msg1.text); // convert student number to uppercase
            strcpy(msg2.text, msg1.text);    // copy msg1 text to msg2 text

            if (strcmp(msg1.student_id, "1211710") == 0)
            {
                strcpy(msg2.student_name, "Goncalo Melo Goncalves");
            }
            else
            {
                strcpy(msg2.student_name, "Aluno desconhecido");
            }

            nbytes = send(new_socket_descriptor, &msg2, sizeof(msg2), 0);
            if (nbytes == -1)
            {
                perror("send message:");
                close(socket_descriptor);
                exit(1);
            }

            printf("Waiting connection\n");
            close(new_socket_descriptor);

            exit(0);
        }
        else
        {
            close(new_socket_descriptor);
        }
    }

    return 0;
}


// converts a string to uppercase and stores it in another string
// str2 is the string where the converted string will be stored
// str1 is the string to be converted
void upperCase(char *str2, char *str1)
{
    for (int i = 0; str1[i] != '\0'; i++)
    {
        str2[i] = toupper(str1[i]);
    }
}

int myReadLine1(int s, char *buf, int count)
{
    int r, n = 0;
    if (count <= 0)
        return 0;
    else if (count == 1)
    {
        buf[0] = 0;
        return 0;
    }
    else
        --count; // leave space for '\0'
    do
    {
        r = read(s, buf + n, 1);
        if (r == 1)
            ++n;
    } while (r == 1 && n < count && buf[n - 1] != '\n');
    buf[n] = '\0';
    return n;
}

void print_address(const struct sockaddr *clt_addr, socklen_t addrlen)
{
    char hostname[256];
    char port[6];

    int n = getnameinfo(clt_addr, addrlen, hostname, sizeof(hostname),
                        port, sizeof(port), NI_NUMERICHOST | NI_NUMERICSERV);
    if (n != 0)
        printf("%s\n", gai_strerror(n));
    else
        printf("Connection from %s:%s\n", hostname, port);
}

int my_create_server_socket(char *port)
{
    int s, r;
    struct addrinfo hints, *a;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    r = getaddrinfo(NULL, port, &hints, &a);
    if (r != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
        exit(1);
    }

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1)
    {
        perror("socket");
        exit(1);
    }

    // avoid bind errors if the port is still being used by previous connections
    int so_reuseaddr = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr));

    r = bind(s, a->ai_addr, a->ai_addrlen);
    if (r == -1)
    {
        perror("bind");
        exit(1);
    }

    r = listen(s, 5);
    if (r == -1)
    {
        perror("listen");
        exit(1);
    }

    freeaddrinfo(a);
    return s;
}
