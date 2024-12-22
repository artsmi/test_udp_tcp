#include <logger.h>
#include <stdio.h>
#include <uthread.h>

#define EXIT_SUCCES (0)
#define EXIT_ERROR (1)

void* simple_thread(void* pparam) {
  int n = 10000;
  while (n > 0) {
    --n;
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
  if (0 != ut_join(pthread->tid)) {
    return EXIT_ERROR;
  }
  ut_destroy(pthread);
  return EXIT_SUCCES;
}
