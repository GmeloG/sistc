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
#include <fcntl.h>

int my_create_server_socket(char *port);
void print_address(const struct sockaddr *clt_addr, socklen_t addrlen);
int myReadLine(int s, char *buf, int count);

int main(int argc, char *argv[])
{

    int new_socket_descriptor, socket_descriptor;
    int nbytes, counter = 0;
    char buffer[4096], studentNumber[10], mensagem[2000], studentName[100];
    char *temp; // temporary variable to store the message

    struct sockaddr clt_addr;
    socklen_t addrlen;

    if (argc != 2)
    {
        printf("Usage: %s port_number\n", argv[0]);
        exit(1);
    }

    signal(SIGPIPE, SIG_IGN); // ignore SIGPIPE

    socket_descriptor = my_create_server_socket(argv[1]);

    while (1)
    {
        char c;

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

            memset(buffer, 0, sizeof(buffer));     // clear array buffer
            memset(mensagem, 0, sizeof(mensagem)); // clear array message
            fflush(stdout);
            nbytes = read(new_socket_descriptor, buffer, sizeof(buffer)); // read message from client
            if (nbytes == -1)
            {
                perror("read");
                break;
            }

            buffer[nbytes] = '\0';
            temp = strtok(buffer, "\n"); // parse the message
            if (temp == NULL)
            {
                printf("Error parsing the student number\n");
            }

            strcpy(studentNumber, temp); // store the student number

            temp = strtok(NULL, "\n"); // parse the student name
            if (temp == NULL)
            {
                printf("Error parsing the student name\n");
            }
            strcpy(mensagem, temp); // store the message
            mensagem[strlen(mensagem)] = '\n';
            for (int i = 0; i < nbytes; i++)
            {
                mensagem[i] = toupper(mensagem[i]);
            }

            if (strcmp(studentNumber, "1211710") == 0)
            {
                strcpy(studentName, "Goncalo Melo Goncalves\n");
            }
            else
            {
                strcpy(studentName, "Aluno desconhecido\n");
            }

            fflush(stdout);
            strcat(mensagem, studentName);
            if (write(new_socket_descriptor, mensagem, strlen(mensagem)) == -1) // send message to client with student name
            {
                perror("write");
                break;
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
