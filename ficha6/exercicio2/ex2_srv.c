#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>

int my_create_server_socket(char *port);
void print_address(const struct sockaddr *clt_addr, socklen_t addrlen);

int main(int argc, char *argv[])
{
    /* Adicionar esta linha no início da função main de cada programa */
    printf("1211710 - %s\n", __FILE__);

    int new_socket_descriptor, socket_descriptor;
    int n;
    struct sockaddr clt_addr;
    socklen_t addrlen;

    if (argc != 2)
    {
        printf("Usage: %s port_number\n", argv[0]);
        exit(1);
    }

    // SIGPIPE is sent to the process if a write is made to a closed connection.
    // By default, SIGPIPE terminates the process. This makes the process to ignore the signal.
    signal(SIGPIPE, SIG_IGN);
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

        // Create a new process for each accepted connection
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }

        // Child process
        if (pid == 0)
        {
            sleep(10); // espera antes de cada leitura
            
            char filename[11];
            strcpy(filename, "fileXXXXXX");
            int fdd = mkstemp(filename);
            if (fdd == -1)
            {
                perror("open destination");
                return 1;
            }

            while (1)
            {

                n = read(new_socket_descriptor, &c, 1);
                if (n <= 0)
                    break;
                
                write(fdd, &c, 1);
            }
            close(new_socket_descriptor);
            close(fdd);

            exit(0); // End the child process
        }
        else
        {
            // Parent process
            close(new_socket_descriptor); // Close the new socket descriptor in the parent process
        }
    }

    close(socket_descriptor);

    return 0;
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
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    r = getaddrinfo(NULL, port, &hints, &a);
    if (r != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
        exit(1);
    }

    s = socket(PF_INET, SOCK_STREAM, 0);
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
