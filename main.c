#include <logger.h>
#include <stdio.h>
#include <uthread.h>

#define EXIT_SUCCES (0)
#define EXIT_ERROR (1)

void* simple_thread(void* pdata) {
  RESOURCES* presources = (RESOURCES*)pdata;
  int n = 100000000;
  while (n > 0 && presources->pf_is_running(presources->pthread_settings)) {
    ulog(LL_I, "pass:%d", --n);
  }
}

int main(int argc, char* argv[]) {
  ulog(LL_I, "The main thread started...\n");

  THREAD_SETTINGS* pthread;
  ut_prepare_thread(&pthread, "Base");
  pthread->pep = &simple_thread;
  pthread->parg_ep = NULL;
  if (0 != ut_create_default(pthread)) {
    return EXIT_ERROR;
  }
  sleep(1);
  ut_cancel(pthread);
  if (0 != ut_join(pthread->tid)) {
    return EXIT_ERROR;
  }
  ut_destroy(pthread);
  return EXIT_SUCCES;
}
