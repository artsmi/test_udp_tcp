#include <logger.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <uthread.h>
#include "../include/net_threads.h"
#include "../include/netdef.h"

void _c_tcp_emergency(void* presources) {
  ulog(LL_I, "TCP Client emergency calling OK");
  shutdown(((RESOURCES*)presources)->data_value, SHUT_RDWR);
}

bool _initiate_connection(int* psocket_desc, RESOURCES* presources) {
  struct sockaddr_in serv_addr;

  *psocket_desc = socket(AF_INET, SOCK_STREAM, 0);
  const int enable = 0;
  if (0 > setsockopt(*psocket_desc, SOL_SOCKET, SO_REUSEADDR, &enable,
                     sizeof(int))) {
    ulog(LL_W, "TCP Client: error setsockopt SO_REUSEADDR: %s",
         strerror(errno));
    return false;
  }
  presources->data_value = *psocket_desc;
  if (*psocket_desc < 0) {
    ulog(LL_E, "TCP Client: error while socker creation: %s", strerror(errno));
    return false;
  }

  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(UDP_SERVER_IP);
  serv_addr.sin_port = htons(TCP_SERVER_PORT);

  if (0 >
      connect(*psocket_desc, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
    ulog(LL_E, "TCP Client: connection was failed...");
    return false;
  }
  ulog(LL_I, "TCP Client: connected to the server..");
  return true;
}

void _rw_client_data_loop(int socket_desc, RESOURCES* presources) {
  char buffer[BUFFER_ALL_DATA_SIZE_BYTES];
  while (utr_is_running(presources)) {
    memset(buffer, '\0', sizeof(buffer));
    int size_bytes = send(socket_desc, buffer, sizeof(buffer), MSG_NOSIGNAL);
    if (size_bytes == 0) {
      ulog(LL_I, "TCP Client: send_err: %d", errno);
      break;
    } else {
      ulog(LL_I, "TCP Client: send: %d", size_bytes);
    }
    usleep(1000000);
    size_bytes = recv(socket_desc, buffer, sizeof(buffer), MSG_DONTWAIT);
    if (size_bytes > 0) {
      ulog(LL_I, "TCP Client: data!");
    } else if (errno == EPIPE) {
      break;
    }
  }
}

void* tcp_client_start(void* pdata) {
  ulog(LL_I, "TCP Client: started..");

  RESOURCES* presources = (RESOURCES*)pdata;
  presources->pf_emergency = _c_tcp_emergency;
  int socket_desc = 0;

  while (utr_is_running(presources)) {
    if (_initiate_connection(&socket_desc, presources)) {
      _rw_client_data_loop(socket_desc, presources);
      close(socket_desc);
    }
    _SLEEP_NETWORK_LOOP_RESTART_;
  }

  ulog(LL_I, "TCP Client: socket closed OK");
  pthread_exit(NULL);
}