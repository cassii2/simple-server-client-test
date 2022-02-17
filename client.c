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

    if (argc != 3) {
        fprintf(stderr, "must pass hostname and port as arguments\n");
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses for %s:\n\n", argv[1]);

    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;

        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        addr = &(ipv4->sin_addr);

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf(" %s\n", ipstr);
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    // if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
    //     fprintf(stderr, "bind() error!\n");
    //     return 3;
    // }
    // bind() isn't necessary for clients, since connect() will auto assign

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        fprintf(stderr, "connect() error: %d\n", h_errno);
        return 4;
    }

    while (1) {
        char *msg = malloc(100 * sizeof(char));
        memset(msg, 0, 100 * sizeof(char));
        int bytes_received = recv(sockfd, (void *) msg, 100 * sizeof(char), 0);
        if (bytes_received == -1) {
            fprintf(stderr, "recv() error!\n");
            return 5;
        }
        if (bytes_received == 0) {
            printf("Connection was closed by remote side\n");
            return 6;
        }
        printf("Message received: %s\n", msg);
    }

    freeaddrinfo(res);

    return 0;
}