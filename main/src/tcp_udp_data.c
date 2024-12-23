#include "../include/tcp_udp_data.h"
#include <string.h>

void tud_prepare(TCP_UDP_DATA* pdata) {
  if (pdata == NULL) {
    return;
  }
  pthread_mutex_init(&pdata->mtx, NULL);
  pthread_cond_init(&pdata->cv, NULL);
  memset(pdata->data, 0, sizeof(pdata->data));
  pdata->nbytes = 0;
  pdata->index = 0;
}

void tud_destroy(TCP_UDP_DATA* pdata) {
  if (pdata == NULL) {
    return;
  }
  pthread_mutex_destroy(&pdata->mtx);
  pthread_cond_destroy(&pdata->cv);
}