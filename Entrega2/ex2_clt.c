#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>

void recv_server_reply1(int);
void recv_server_reply2(int);
int my_connect(char *servername, char *port);
void print_socket_address(int sd);

int main(int argc, char *const argv[])
{
    /* Adicionar esta linha no início da função main de cada programa */
    printf("1211710 - %s\n", __FILE__);

    char buffer[4096];

    if (argc != 4)
    {
        printf("usage: %s <server ip> <porto> <file name>\n", argv[0]);
        return 1;
    }

    // open file
    int fds = open(argv[3], O_RDONLY);
    if (fds == -1)
    {
        perror("open source");
        return 1;
    }

    // connect to server
    int socket_descriptor = my_connect(argv[1], argv[2]); // connect to server

    // print local address
    print_socket_address(socket_descriptor);

    // read from file and write to socket
    while (1)
    {
        // read from file
        int n = read(fds, buffer, sizeof(buffer));
        if (n <= 0) // means the end of file was reached
            break;

        // write buffer contents to socket
        write(socket_descriptor, buffer, n);
    }

    // close file and socket
    close(fds);
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
    int s = socket(AF_INET, SOCK_STREAM, 0); // TCP socket (SOCK_STREAM)
    if (s == -1)
    {
        perror("socket");
        exit(2);
    }

    // connect to server
    r = connect(s, addrs->ai_addr, addrs->ai_addrlen); // connect to server
    if (r == -1)
    {
        perror("connect");
        close(s);
        exit(3);
    }

    freeaddrinfo(addrs);
    return s;
}

// version 1 - prints server reply as it receives it (by parts)
void recv_server_reply1(int s)
{
    char buf[4096];

    printf("Reply from server: ");
    while (1)
    {
        int n = read(s, buf, sizeof(buf) - 1);
        if (n == 0)
            break;
        buf[n] = 0; // terminate string (read does not do this)
        printf(buf);
        fflush(stdout);
    }

    printf("\n\n");
}

// version 2 - receives whole answer and then prints it
void recv_server_reply2(int s)
{
    char buf[4096];

    int bytes_recv = 0;
    while (1)
    {
        int n = read(s, buf + bytes_recv, sizeof(buf) - 1 - bytes_recv);
        if (n == 0)
            break;

        bytes_recv += n;
    }

    buf[bytes_recv] = 0; // terminate string (read does not do this)
    printf("Reply from server: %s\n\n", buf);
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
