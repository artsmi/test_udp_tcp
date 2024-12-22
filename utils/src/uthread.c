#include "../include/uthread.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/logger.h"

bool ut_verificate(const THREAD_SETTINGS* pts) {
  return pts != NULL && pts->pname != NULL && pts->pep != NULL;
}

void ut_prepare_thread(THREAD_SETTINGS** ppts, const char* pname) {
  size_t size_bytes = sizeof(THREAD_SETTINGS);
  *ppts = malloc(size_bytes);
  memset(*ppts, 0, size_bytes);
  (*ppts)->pname = pname;
}

void ut_destroy(THREAD_SETTINGS* pts) {
  if (pts == NULL) {
    return;
  }
  int errn = pthread_attr_destroy(&pts->attr);
  if (errn != 0) {
    ulog(LL_E, "cannot destroy attr thread %s, E: %s", pts->pname,
         strerror(errn));
  }
}

int ut_create_default(THREAD_SETTINGS* pts) {
  if (!ut_verificate(pts)) {
    ulog(LL_E, "cannot create thread %s, no valid", pts->pname);
    return EPERM;
  }
  pts->tid = 0;
  pthread_attr_init(&pts->attr);
  int errn = pthread_create(&pts->tid, &pts->attr, pts->pep, pts->parg_ep);
  if (errn != 0) {
    ulog(LL_E, "cannot create thread %s, E: %s", pts->pname, strerror(errn));
  } else {
    ulog(LL_I, "thread created %s, %p", pts->pname, (void*)pts->tid);
  }
  return errn;
}

int ut_join(pthread_t tid) {
  int errn = pthread_join(tid, NULL);
  if (errn == ESRCH) {
    ulog(LL_W, "%p thread could not be found for join (ESRCH)", (void*)tid);
    errn = 0;
  } else if (errn == 0) {
    ulog(LL_I, "joined with %p thread", (void*)tid);
  } else {
    ulog(LL_E, "failed to join %p thread: %s", (void*)tid, strerror(errn));
  }
  return errn;
}