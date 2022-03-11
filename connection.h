#ifndef CONNECTION_H_
#define CONNECTION_H_

enum IPINFO { IPV4, IPV6 };
#define BACKLOG 10

// Sets up new socket for incoming connections
int init_connection(enum IPINFO ipv, const char *port);
void deinit_connection(int sock);

// Waits for new connection and returns socket
int get_newconnection(int sock);

// Returns a string and puts the size into "size"
char *receive_data(int sock, int *size);
// Sends data to sock from "data" of size "size"
void send_data(int sock, char *data, unsigned int size);

#endif // CONNECTION_H_
