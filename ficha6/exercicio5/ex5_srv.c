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
#include <sys/select.h>
#include <string.h>

// # function prototypes #//
int my_create_server_socket(char *port);
void print_address(const struct sockaddr *clt_addr, socklen_t addrlen);
int myReadLine(int s, char *buf, int count);
void upperCase(char *str2, char *str1);

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
    char buffer[4096];
    int nbytes;
    char nbytes_str;
    char num_bytes[10];
    char *text;

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

            struct timeval timeout;
            timeout.tv_sec = 5;
            timeout.tv_usec = 0;

            // Configurando o conjunto de descritores de arquivo para select
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(new_socket_descriptor, &read_fds);

            // Chamando select para verificar se há dados prontos para serem lidos
            while (select(new_socket_descriptor + 1, &read_fds, NULL, NULL, &timeout) <= 0)
            {
                printf("Mensagem nao recebida em 5s\n");
                close(new_socket_descriptor);
                exit(1);
            }

            // Receber mensagem do cliente
            if (read(new_socket_descriptor, &buffer, sizeof(buffer)) == -1)
            {
                perror("error write\n");
                close(socket_descriptor);
                exit(1);
            }
            //printf("Mensagem recebida do cliente: %s\n", buffer);

            // Preencher msg1.student_id e num_bytes
            sscanf(buffer, "%7s%s\n", msg1.student_id, num_bytes);
            int num_bytes_str = strlen(msg1.student_id) + strlen(num_bytes) + 1;

            // Preencher msg1.text
            for (int i = 0; i < atoi(num_bytes) + 1; i++)
            {
                msg1.text[i] = buffer[num_bytes_str + i];
            }
            
            // Preencher msg2.student_name
            if (strncmp(msg1.student_id, "1211710", 7) == 0) // string compare with 7 characters to avoid the \n
            {
                strcpy(msg2.student_name, "Goncalo Melo Goncalves");
            }
            else
            {
                strcpy(msg2.student_name, "Aluno desconhecido");
            }
            // Preencher msg2.text para maiosculas
            upperCase(msg2.text, msg1.text); // convert student number to uppercase

            // Limpar buffer
            memset(buffer, 0, sizeof(buffer)); // clear array buffer

            // Preencher variavel sprintf
            sprintf(buffer, "%ld\n%s%ld\n%s", strlen(msg2.text), msg2.text, strlen(msg2.student_name), msg2.student_name);

            // Imprimir mensagem a enviar
            printf("Mensagem a enviar para o cliente: %s\n", buffer);

            // sending to server
            if (write(new_socket_descriptor, buffer, sizeof(buffer)) == -1)
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
