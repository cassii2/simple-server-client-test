#include "connection.h"

#include <asm-generic/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef linux
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#define SHUTDOWNOPT SHUT_RDWR
#elif defined(_WIN32)
#include <WinSock2.h>
#include <ws2def.h>
#define SHUTDOWNOPT SD_BOTH
#endif

int init_connection(enum IPINFO ipv, const char *port) {
  struct addrinfo hints, *res;
  int status;
  memset(&hints, 0, sizeof hints);

  hints.ai_family = (ipv != IPV6 ? AF_INET : AF_INET6);
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    exit(2);
  }

  int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  int yes = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
    perror("setsockopt");
    exit(1);
  }

  if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
    fprintf(stderr, "bind() error!\n");
    exit(3);
  }

  if (listen(sock, BACKLOG) == -1) {
    fprintf(stderr, "listen() error!\n");
    exit(4);
  }

  freeaddrinfo(res);
  return sock;
}

// Waits for a connection to SOCKET, and returns the new socket for that
// connection
int get_newconnection(int sock) {
  struct sockaddr_storage curr_addr;
  unsigned int addr_size = sizeof curr_addr;

  int newsock = accept(sock, (struct sockaddr *)&curr_addr, &addr_size);
  if (newsock == -1) {
    fprintf(stderr, "accept() error!\n");
    exit(5);
  }

  return newsock;
}

void send_data(int sock, char *data, unsigned int size) {
  unsigned int bytes_left = size;
  while (bytes_left != 0) {
    unsigned int bytes_sent =
        send(sock, data + (size - bytes_left), bytes_left, 0);
    bytes_left -= bytes_sent;
  }
}
char *receive_data(int sock, int *size) {
  char *target = (char *)calloc(8, sizeof(char));

  unsigned int bytes_received = 1;
  unsigned int count = 8;
  while (bytes_received != 0) {
    target = (char *)realloc(target, sizeof(char) * count);
    memset(target + count, 0, strlen(target) - count);
    unsigned int bytes_received = recv(sock, target, sizeof(char) * 8, 0);
    count += bytes_received;
  }

  *size = count;
  return target;
}
