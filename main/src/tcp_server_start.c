#include <fcntl.h>
#include <logger.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <uthread.h>
#include "../include/net_threads.h"
#include "../include/netdef.h"

void _s_tcp_emergency(void* presources) {
  ulog(LL_I, "TCP Server emergency calling OK");
  shutdown(((RESOURCES*)presources)->data_value, SHUT_RDWR);
}

bool _create_socket(int* psocket_desc, RESOURCES* presources) {
  struct sockaddr_in serv_addr;

  *psocket_desc = socket(AF_INET, SOCK_STREAM, 0);
  ulog(LL_I, "TCP Server: socket created: %d", *psocket_desc);
  presources->data_value = *psocket_desc;
  if (*psocket_desc < 0) {
    ulog(LL_W, "TCP Server: error while socker creation: %s", strerror(errno));
    return false;
  }
  fcntl(*psocket_desc, F_SETFL, O_NONBLOCK);

  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(TCP_SERVER_PORT);

  if (0 >
      bind(*psocket_desc, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
    ulog(LL_W, "TCP Server: error while socket binding: %s", strerror(errno));
    return false;
  }
  if ((listen(*psocket_desc, NMAX_TCP_CONNECTIONS)) != 0) {
    ulog(LL_W, "TCP Server: error while socker listening: %s", strerror(errno));
    return false;
  }
  ulog(LL_I, "TCP Server: socket bind OK, listening...");
}

bool _try_accept_connection(int socket_desc, int* pconn_desc) {
  struct sockaddr_in client_addr;
  int client_addr_size = sizeof(client_addr);
  *pconn_desc =
      accept(socket_desc, (struct sockaddr*)&client_addr, &client_addr_size);
  if (*pconn_desc < 0) {
    return false;
  }
  ulog(LL_I, "TCP Server: accepted the client: %d", *pconn_desc);
  return true;
}

void _rw_srv_data_loop(int socket_desc, RESOURCES* presources) {
  char buffer[BUFFER_ALL_DATA_SIZE_BYTES];
  memset(buffer, '\0', sizeof(buffer));
  int conn_desc = 0;
  while (utr_is_running(presources)) {
    int new_conn_desc = 0;
    if (_try_accept_connection(socket_desc, &new_conn_desc)) {
      close(conn_desc);
      conn_desc = new_conn_desc;
    }
    if (conn_desc != 0) {
      int size_bytes = recv(conn_desc, buffer, sizeof(buffer), MSG_DONTWAIT);
      if (size_bytes > 0) {
        ulog(LL_I, "TCP Server: data: buffer len: %d, size_bytes: %d",
             strlen(buffer), size_bytes);
        send(conn_desc, buffer, sizeof(buffer) /*strlen(buffer)*/,
             MSG_NOSIGNAL);
      } else if (errno == EPIPE) {
        break;
      }
    }
    _SLEEP_NETWORK_SHORT_;
  }
  if (conn_desc != 0)
    close(conn_desc);
}

void* tcp_server_start(void* pdata) {
  ulog(LL_I, "TCP Server: started..");

  RESOURCES* presources = (RESOURCES*)pdata;
  presources->pf_emergency = _s_tcp_emergency;
  int socket_desc = 0;

  _create_socket(&socket_desc, presources);

  while (utr_is_running(presources)) {
    _rw_srv_data_loop(socket_desc, presources);
    _SLEEP_NETWORK_LOOP_RESTART_;
  }

  close(socket_desc);
  ulog(LL_I, "TCP Server: socket closed OK");
  pthread_exit(NULL);
}