#ifndef TCP_UDP_DATA_H
#define TCP_UDP_DATA_H

#include <pthread.h>
#include "../include/netdef.h"

typedef struct _TCP_UDP_DATA {
  pthread_mutex_t mtx;
  pthread_cond_t cv;
  char data[BUFFER_ALL_DATA_SIZE_BYTES];
  size_t nbytes;
  int index;
} TCP_UDP_DATA;

extern void tud_prepare(TCP_UDP_DATA* pdata);
extern void tud_destroy(TCP_UDP_DATA* pdata);

#endif  // TCP_UDP_DATA_H