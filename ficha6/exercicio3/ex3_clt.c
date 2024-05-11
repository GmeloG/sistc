#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>

#define bufferSize 2000

int my_connect(char *servername, char *port);
void print_socket_address(int sd);

int main(int argc, char *const argv[])
{
    char studentNumber[] = "1211710\n", message[bufferSize];
    char *line1, *line2;
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

    // write student number to socket
    nbytes = write(socket_descriptor, studentNumber, sizeof(studentNumber));
    if (nbytes == -1)
    {
        perror("write message:");
        close(socket_descriptor);
        exit(1);
    }

    // read from stdin the message from the user
    do
    {
        fflush(stdout); // clear output buffer
        printf("\n");   
        printf("Intruduza a messagem a enviar:\n");

        int ch; // character read from stdin
        int i = 0;
        while ((ch = getchar()) != '\n' && ch != EOF)   // read until newline or EOF
        {
            if (i < sizeof(message) - 1)    // check if there is space in the buffer
            {
                message[i] = ch;
                i++;
            }
        }
        message[i] = '\n';

        if (strlen(message) <= 1)   // check if the message is empty
        {
            continue;
        }

        break;
    } while (1);

    // write message contents to socket 
    nbytes = write(socket_descriptor, message, strlen(message)); 
    if (nbytes == -1)
    {
        perror("write student number:");
        close(socket_descriptor);
        exit(1);
    }

    // Receiving response from server
    char response[bufferSize];
    nbytes = read(socket_descriptor, response, sizeof(response));
    if (nbytes == -1)
    {
        perror("read from socket");
        close(socket_descriptor);
        exit(1);
    }
    response[nbytes] = '\0'; // null terminate the string

    line1 = strtok(response, "\n"); // parse the message
    if (line1 == NULL)
    {
        printf("Error parsing the message\n");
    }

    line2 = strtok(NULL, "\n"); // parse the student name
    if (line2 == NULL)
    {
        printf("Error parsing the student name\n");
    }

    printf("message: %s\n"
           "Nome: %s\n"
           "Total: %d\n",
           line1, line2, nbytes);

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
