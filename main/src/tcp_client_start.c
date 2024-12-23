#include <logger.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <uthread.h>
#include "../include/net_threads.h"
#include "../include/netdef.h"
#include "../include/tcp_udp_data.h"

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
  TCP_UDP_DATA* pres_data = (TCP_UDP_DATA*)presources->parg_ep;
  if (pres_data == NULL) {
    ulog(LL_E, "TCP Client: no data buffer");
    return;
  }
  char buffer_recv[BUFFER_ALL_DATA_SIZE_BYTES];
  size_t count = 0;
  int last_index = 0;
  while (utr_is_running(presources)) {
    int size_bytes = 0;
    pthread_mutex_lock(&pres_data->mtx);
    if (pres_data->nbytes > BUFFER_ADD_DATA_SIZE_BYTES &&
        last_index != pres_data->index) {
      // pthread_cond_wait(&pres_data->cv, &pres_data->mtx);
      last_index = pres_data->index;
      size_bytes =
          send(socket_desc, pres_data->data, pres_data->nbytes, MSG_NOSIGNAL);
      if (size_bytes == 0) {
        ulog(LL_I, "TCP Client: send_err: %d", errno);
        break;
      }
    }
    pthread_mutex_unlock(&pres_data->mtx);
    size_bytes =
        recv(socket_desc, buffer_recv, sizeof(buffer_recv), MSG_DONTWAIT);
    if (size_bytes > 0) {
      ++count;
      if ((count % _N_PACKETS_BACK_) == 0) {
        ulog(LL_I, "TCP Client: get %d packets back!", _N_PACKETS_BACK_);
        count = 0;
      }
    } else if (errno == EPIPE) {
      break;
    }
    _SLEEP_NETWORK_SHORT_RECV_;
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