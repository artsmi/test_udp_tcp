#include <logger.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "../include/net_threads.h"
#include "../include/thread_mgr.h"

#define EXIT_SUCCES (0)
#define EXIT_ERROR (1)

static THREAD_MANAGER gtmgr;
void interrupt_function(int sig) {
  ulog(LL_I, "Emergency threads cancelling...");
  thm_emergency_all(&gtmgr);
  thm_cancell_all(&gtmgr);
}

int main(int argc, char* argv[]) {
  ulog(LL_I, "The main thread started...");

  thm_prepare(&gtmgr);
  thm_add_thread(&gtmgr, "SimpleSample", &simple_sample_start, NULL);

  THREAD_SETTINGS* pts_tcpserver =
      thm_add_thread(&gtmgr, "(S)TCP_Server", &tcp_server_start, NULL);
  THREAD_SETTINGS* pts_tcpclient =
      thm_add_thread(&gtmgr, "(S)TCP_Client", &tcp_client_start, NULL);

  thm_add_thread(&gtmgr, "(C)UDP_Pusher", &udp_client_pusher_start, NULL);
  thm_add_thread(&gtmgr, "(S)UDP_Server", &udp_server_start, NULL);

  signal(SIGINT, interrupt_function);
  signal(SIGKILL, interrupt_function);

  usleep(3000000);
  ulog(LL_I, "Crashing TCP Client...");
  ut_cancel(pts_tcpclient);
  ut_join(pts_tcpclient);

  usleep(3000000);
  ulog(LL_I, "Starting TCP Client...");
  thm_add_thread(&gtmgr, "(S)TCP_Client 2", &tcp_client_start, NULL);

  usleep(3000000);
  ulog(LL_I, "Crashing TCP Server...");
  ut_cancel(pts_tcpserver);
  ut_join(pts_tcpserver);

  usleep(3000000);
  ulog(LL_I, "Starting TCP Server...");
  thm_add_thread(&gtmgr, "(S)TCP_Server 2", &tcp_server_start, NULL);

  thm_wait_all(&gtmgr);
  thm_destroy_all(&gtmgr);

  return EXIT_SUCCES;
}
