#include <logger.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "../include/net_threads.h"
#include "../include/tcp_udp_data.h"
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

  TCP_UDP_DATA data;
  tud_prepare(&data);
  thm_prepare(&gtmgr);

  thm_add_thread(&gtmgr, "(S)TCP_Client", &tcp_client_start, (void*)&data);
  thm_add_thread(&gtmgr, "(S)UDP_Server", &udp_server_start, (void*)&data);

  signal(SIGINT, interrupt_function);
  signal(SIGKILL, interrupt_function);

  thm_wait_all(&gtmgr);
  thm_destroy_all(&gtmgr);

  return EXIT_SUCCES;
}
