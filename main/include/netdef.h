#ifndef NETDEF_H
#define NETDEF_H

#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>

#define TCP_SERVER_PORT (2001)
#define NMAX_TCP_CONNECTIONS (2)
#define UDP_SERVER_PORT (2000)
#define UDP_SERVER_IP ("127.0.0.1")

#define BUFFER_DATA_MIN_SIZE_BYTES (16)
#define BUFFER_DATA_MAX_SIZE_BYTES (128)
#define BUFFER_ADD_DATA_SIZE_BYTES (4)
#define BUFFER_ALL_DATA_SIZE_BYTES \
  (BUFFER_DATA_MAX_SIZE_BYTES + BUFFER_ADD_DATA_SIZE_BYTES)

#define _SLEEP_NETWORK_LOOP_RESTART_ usleep(3000000)
#define _SLEEP_NETWORK_SHORT_ usleep(500000)

#endif  // NETDEF_H
