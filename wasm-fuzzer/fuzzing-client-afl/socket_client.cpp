#include "socket_client.h"

// Most code based on: https://www.bogotobogo.com/cplusplus/sockets_server_client.php

void error(const char *msg)
{
    // fprintf("Socket: %s\n", msg);
    perror(msg);
    throw std::runtime_error(msg);
}

int connectToServer(char *socket_hostname, int socket_port)
{
    int sockfd;
    struct hostent *server;
    struct sockaddr_in serv_addr;

    /* 
        SOCK_STREAM: TCP (reliable, connection oriented)
        SOCK_DGRAM: UDP (unreliable, connectionless)
        AF_INET: IPv4 protocol
        AF_INET6: IPv6 protocol 
    */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        error("Error opening socket");

    server = gethostbyname(socket_hostname);

    if (server == NULL)
    {
        // fprintf(stderr, "Error, no such host\n");
        perror("Error, no such host");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr)); // Maybe use memset

    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);

    serv_addr.sin_port = htons(socket_port);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Error connecting");

    return sockfd;
}

void clientWrite(int sockfd, char *sendBuffer, int sizeBuffer)
{
    int n = write(sockfd, sendBuffer, sizeBuffer);
    if (n < 0)
        error("Error writing to socket");
}

void clientRead(int sockfd, char *readBuffer, int sizeBuffer)
{
    int n = 0;
    while (n < sizeBuffer)
    {
        // Occasionally, read() finishes reading before all the data has
        // been written to the socket by the SWAM server. Therefore, this
        // is in a loop.
        int nNew = read(sockfd, readBuffer + n, sizeBuffer - n);
        if (nNew < 0)
            error("Error reading from socket");
        n = n + nNew;
    }
}

void runClient(
    int sizeSendBuffer,
    char *sendBuffer,
    int sizeReadBuffer,
    char *readBuffer,
    char *socket_hostname,
    int socket_port)
{
    int sockfd = connectToServer(socket_hostname, socket_port);
    clientWrite(sockfd, sendBuffer, sizeSendBuffer);
    clientRead(sockfd, readBuffer, sizeReadBuffer);
    close(sockfd);
};