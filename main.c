#include <errno.h>
#include <logger.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define EXIT_SUCCES (0)
#define EXIT_ERROR (1)

void simple_thread(void* pparam) {
  while (1) {
    // printf("done\n");
  }
}

int main(int argc, char* argv[]) {
  ulog(LL_I, "The main C Linux program\n");

  pthread_t tid = 0;
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  int errn = pthread_create(&tid, &attr, (void*)&simple_thread, argv[1]);
  if (0 != errn) {
    // strerror();
  }

  pthread_join(tid, NULL);

  return EXIT_SUCCES;
}
