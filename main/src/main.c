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

/*int main_sample(THREAD_MANAGER* ptmgr) {
  for (int i = 0; i < THM_MAX_THREAD_SUPPORT; ++i) {
    if (NULL ==
        thm_add_thread(ptmgr, "SimpleSample", &simple_sample_start, NULL)) {
      ulog(LL_E, "Bad thread initlializng... %d", i);
      break;
    }
  }
  usleep(1000000);
  thm_stop_destroy_all(ptmgr);
}*/

int main(int argc, char* argv[]) {
  ulog(LL_I, "The main thread started...");

  thm_prepare(&gtmgr);
  thm_add_thread(&gtmgr, "(C)UDP_Pusher", &udp_client_pusher_start, NULL);
  thm_add_thread(&gtmgr, "(C)UDP_Pusher", &udp_client_pusher_start, NULL);
  thm_add_thread(&gtmgr, "(C)UDP_Pusher", &udp_client_pusher_start, NULL);
  thm_add_thread(&gtmgr, "(C)UDP_Pusher", &udp_client_pusher_start, NULL);
  thm_add_thread(&gtmgr, "(C)UDP_Pusher", &udp_client_pusher_start, NULL);
  thm_add_thread(&gtmgr, "(C)UDP_Pusher", &udp_client_pusher_start, NULL);
  thm_add_thread(&gtmgr, "(C)UDP_Pusher", &udp_client_pusher_start, NULL);
  thm_add_thread(&gtmgr, "(C)UDP_Pusher", &udp_client_pusher_start, NULL);
  thm_add_thread(&gtmgr, "(S)UDP_Server", &udp_server_start, NULL);

  signal(SIGINT, interrupt_function);
  signal(SIGKILL, interrupt_function);

  thm_wait_all(&gtmgr);
  thm_destroy_all(&gtmgr);

  return EXIT_SUCCES;
}
