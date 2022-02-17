#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char **argv) {
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET_ADDRSTRLEN];


    if (argc < 2) {
        fprintf(stderr, "must pass port as argument\n");
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, argv[1], &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        perror("setsockopt");
        exit(1);
    }

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        fprintf(stderr, "bind() error!\n");
        return 3;
    }

    int backlog = 10;
    if (listen(sockfd, backlog) == -1) {
        fprintf(stderr, "listen() error!\n");
        return 4;
    }

    struct sockaddr_storage curr_addr;
    unsigned int addr_size = sizeof curr_addr;
    int newfd = accept(sockfd, (struct sockaddr *)&curr_addr, &addr_size);
    if (newfd == -1) {
        fprintf(stderr, "accept() error!\n");
        return 5;
    }

    char *msg = malloc(sizeof(char) * 100);
    memset(msg, 0, sizeof(char) * 100);
    if (argc == 2) 
        msg = "You connected! Congrats! :D";
    else if (argc == 3)
        msg = argv[2];
    else 
        msg = "wow how did you get here";
    int bytes_left = strlen(msg);
    while (bytes_left != 0) {
        char *uhh = malloc(sizeof(char) * 100);
        recv(newfd, uhh, sizeof(char) * 100, 0);
        int bytes_sent = send(newfd, msg + (strlen(msg) - bytes_left), bytes_left, 0);
        if (bytes_sent != -1) {
            printf("Message sent: %s\n", msg + (strlen(msg) - bytes_sent));
        }
        bytes_left -= bytes_sent;
        printf("Bytes sent: %d\nBytes left: %d\n", bytes_sent, bytes_left);
    }
    shutdown(newfd, SHUT_RDWR);

    freeaddrinfo(res);

    return 0;
}