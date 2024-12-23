#include <logger.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <uthread.h>
#include "../include/net_threads.h"
#include "../include/netdef.h"
#include "../include/tcp_udp_data.h"

void s_udp_emergency(void* presources) {
  ulog(LL_I, "UDP Server emergency calling OK");
  shutdown(((RESOURCES*)presources)->data_value, SHUT_RDWR);
}

void _add_const_data(TCP_UDP_DATA* pres_data, const char* p4data) {
  pthread_mutex_lock(&pres_data->mtx);
  memcpy(pres_data->data, p4data, BUFFER_ADD_DATA_SIZE_BYTES);
  pthread_mutex_unlock(&pres_data->mtx);
}

void* udp_server_start(void* pdata) {
  ulog(LL_I, "UDP Server thread started...");

  const char* p4data = "ABCD";
  RESOURCES* presources = (RESOURCES*)pdata;
  TCP_UDP_DATA* pres_data = (TCP_UDP_DATA*)presources->parg_ep;
  if (pres_data == NULL) {
    ulog(LL_E, "UDP Server no data buffer");
    pthread_exit(NULL);
  }
  _add_const_data(pres_data, p4data);
  presources->pf_emergency = s_udp_emergency;
  int socket_desc = 0;
  struct sockaddr_in server_addr;

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

  while (utr_is_running(presources)) {
    char* pmessage = pres_data->data + BUFFER_ADD_DATA_SIZE_BYTES;
    pthread_mutex_lock(&pres_data->mtx);
    memset(pmessage, 0, BUFFER_DATA_MAX_SIZE_BYTES);
    int size_bytes =
        recv(socket_desc, pmessage, BUFFER_DATA_MAX_SIZE_BYTES, MSG_DONTWAIT);
    if (0 > size_bytes) {
      if (errno != EAGAIN)
        ulog(LL_E, "UDP Server: error recv data: %d: %s", errno,
             strerror(errno));
    } else {
      // ulog(LL_I, "UDP Server: got data: %d", size_bytes);
      pres_data->nbytes = size_bytes + BUFFER_ADD_DATA_SIZE_BYTES;
      ++(pres_data->index);
      if (pres_data->index == __INT_MAX__ - 1) {
        pres_data->index = 0;
      }
      // pthread_cond_signal(&pres_data->cv);
    }
    pthread_mutex_unlock(&pres_data->mtx);
    _SLEEP_NETWORK_SHORT_RECV_;
  }
  close(socket_desc);
  shutdown(socket_desc, SHUT_RDWR);
  ulog(LL_I, "UDP Server: socket closed OK");
  pthread_exit(NULL);
}