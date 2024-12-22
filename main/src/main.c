#include <logger.h>
#include <stdio.h>
#include <unistd.h>

#include "../include/net_threads.h"
#include "../include/thread_mgr.h"

#define EXIT_SUCCES (0)
#define EXIT_ERROR (1)

int main(int argc, char* argv[]) {
  ulog(LL_I, "The main thread started...");

  THREAD_MANAGER tmgr;
  thm_prepare(&tmgr);
  for (int i = 0; i < THM_MAX_THREAD_SUPPORT; ++i) {
    if (NULL ==
        thm_add_thread(&tmgr, "SimpleSample", &simple_sample_start, NULL)) {
      ulog(LL_E, "Bad thread initlializng... %d", i);
      break;
    }
  }
  usleep(1000000);
  thm_stop_destroy_all(&tmgr);
  return EXIT_SUCCES;
}
