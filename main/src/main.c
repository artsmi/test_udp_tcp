#include <logger.h>
#include <stdio.h>
#include <uthread.h>
#include <unistd.h>
#include "../include/net_threads.h"

#define EXIT_SUCCES (0)
#define EXIT_ERROR (1)

int main(int argc, char* argv[]) {
  ulog(LL_I, "The main thread started...");

  THREAD_SETTINGS* pthread;
  ut_prepare_thread(&pthread, "Base");
  pthread->pep = &simple_sample_start;
  pthread->parg_ep = NULL;
  if (0 != ut_create_default(pthread)) {
    return EXIT_ERROR;
  }
  usleep(1000000);
  ut_cancel(pthread);
  if (0 != ut_join(pthread->tid)) {
    return EXIT_ERROR;
  }
  ut_destroy(pthread);
  return EXIT_SUCCES;
}
