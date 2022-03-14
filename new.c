#include "connection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s [port] [file]\n", argv[0]);
    return 0;
  }

  char *port = argv[1];
  char *file = argv[2];

  int socket = init_connection(IPV4, port);
  // TODO: Actually make it respond to the data the browser sends
  char *predata = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";

  FILE *fs = fopen(argv[2], "r");
  fseek(fs, 0, SEEK_END);
  int htmlsize = ftell(fs);
  rewind(fs);
  char *html = malloc(sizeof(char) * htmlsize);
  fread(html, sizeof(char), htmlsize, fs);

  int newsock = get_newconnection(socket);
  char *data = receive_data(newsock, NULL);
  printf("Received:-------\n%s\n----------------\n", data);
  free(data);
  send_data(newsock, predata, strlen(predata));
  send_data(newsock, html, htmlsize);

  // TODO: cleanup/close newsock and sock
}
