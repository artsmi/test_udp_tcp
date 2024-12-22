#include <logger.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <uthread.h>
#include "../include/net_threads.h"
#include "../include/netdef.h"

void s_udp_emergency(void* presources) {
  ulog(LL_I, "UDP Server emergency calling OK");
  shutdown(((RESOURCES*)presources)->data_value, SHUT_RDWR);
}

void* udp_server_start(void* pdata) {
  ulog(LL_I, "UDP Server thread started...");

  const char* padd_data = "ABCD";
  RESOURCES* presources = (RESOURCES*)pdata;
  presources->pf_emergency = s_udp_emergency;
  int socket_desc = 0;
  struct sockaddr_in server_addr, client_addr;
  int client_struct_length = sizeof(client_addr);
  char client_message[BUFFER_ALL_DATA_SIZE_BYTES];
  char* pmessage = client_message + BUFFER_ADD_DATA_SIZE_BYTES;

  socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  presources->data_value = socket_desc;
  if (socket_desc < 0) {
    ulog(LL_E, "UDP Server: error while socker creation: %s", strerror(errno));
    pthread_exit(NULL);
  }
  ulog(LL_I, "UDP Server: socket created OK");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(UDP_SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr(UDP_SERVER_IP);

  if (0 >
      bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
    ulog(LL_E, "UDP server: error while socker binding: %s", strerror(errno));
    pthread_exit(NULL);
  }
  ulog(LL_I, "UDP Server: socket bind OK, listening...");

  size_t msize = BUFFER_DATA_MAX_SIZE_BYTES;
  while (utr_is_running(presources)) {
    memset(pmessage, '\0', msize);
    if (0 > recvfrom(socket_desc, pmessage, msize, 0,
                     (struct sockaddr*)&client_addr, &client_struct_length)) {
      ulog(LL_E, "UDP Server: recv data: %s", strerror(errno));
      memset(pmessage, '\0', BUFFER_DATA_MAX_SIZE_BYTES);
    } else {
      memcpy(client_message, padd_data, BUFFER_ADD_DATA_SIZE_BYTES);
      /*ulog(LL_I, "UDP Server: Received message from IP: %s and port: %i, with
         size: %d", inet_ntoa(client_addr.sin_addr),
         ntohs(client_addr.sin_port), last_size);*/
    }
  }
  close(socket_desc);
  shutdown(socket_desc, SHUT_RDWR);
  ulog(LL_I, "UDP Server: socket closed OK");
  pthread_exit(NULL);
}