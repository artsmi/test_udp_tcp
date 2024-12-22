#include <logger.h>
#include <uthread.h>
#include "../include/net_threads.h"

void* simple_sample_start(void* pdata) {
  ulog(LL_I, "Simple Sample thread started...");
  RESOURCES* presources = (RESOURCES*)pdata;
  int n = 100000000;
  while (n > 0 && presources->pf_is_running(presources->pthread_settings)) {
    // ulog(LL_I, "pass:%d", --n);
    --n;
  }
  pthread_exit(NULL);
}