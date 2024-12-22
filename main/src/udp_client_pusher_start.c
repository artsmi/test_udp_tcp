#include <logger.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <uthread.h>
#include "../include/net_threads.h"
#include "../include/netdef.h"

size_t get_random_size() {
  srand((unsigned)time(NULL));
  return (rand() % (BUFFER_DATA_MAX_SIZE_BYTES - BUFFER_DATA_MIN_SIZE_BYTES)) +
         BUFFER_DATA_MIN_SIZE_BYTES;
}

void* udp_client_pusher_start(void* pdata) {
  ulog(LL_I, "UDP Client Pusher thread started...");
  RESOURCES* presources = (RESOURCES*)pdata;

  int socket_desc = 0;
  struct sockaddr_in server_addr;
  int server_struct_length = sizeof(server_addr);
  memset(&server_addr, 0, server_struct_length);
  char data[BUFFER_DATA_MAX_SIZE_BYTES];

  socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socket_desc < 0) {
    ulog(LL_E, "client: error while socker creation: %s", strerror(errno));
    pthread_exit(NULL);
  }
  ulog(LL_I, "Client: socket created OK");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(UDP_SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr(UDP_SERVER_IP);

  size_t last_random_size = sizeof(data);
  while (presources->pf_is_running(presources->pthread_settings)) {
    memset(data, '\0', last_random_size);
    last_random_size = get_random_size();
    for (size_t i = 0; i < last_random_size; i++)
      data[i] = rand();
    if (0 > sendto(socket_desc, data, last_random_size, 0,
                   (struct sockaddr*)&server_addr, server_struct_length)) {
      ulog(LL_E, "client: send data error: %s", strerror(errno));
    } else {
      ulog(LL_I, "client: send data: %d", last_random_size);
    }
    usleep(1000000);
  }
  close(socket_desc);
  pthread_exit(NULL);
}