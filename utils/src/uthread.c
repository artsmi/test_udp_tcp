#include "../include/uthread.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/logger.h"

bool ut_verificate(const THREAD_SETTINGS* pts) {
  return pts != NULL && pts->pname != NULL && pts->pep != NULL &&
         pts->resources.pf_is_running != NULL;
}

static bool _utf_is_running(void* pthread_settings) {
  return ut_is_running((THREAD_SETTINGS*)pthread_settings);
}

void ut_prepare_thread(THREAD_SETTINGS** ppts, const char* pname) {
  size_t size_bytes = sizeof(THREAD_SETTINGS);
  *ppts = malloc(size_bytes);
  memset(*ppts, 0, size_bytes);
  (*ppts)->pname = pname;
  (*ppts)->resources.pf_is_running = _utf_is_running;
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
  pthread_mutex_destroy(&pts->mtx_is_done);
  free(pts);
}

int ut_create_default(THREAD_SETTINGS* pts) {
  if (!ut_verificate(pts)) {
    ulog(LL_E, "cannot create thread %s, no valid", pts->pname);
    return EPERM;
  }
  pts->is_done = false;
  pthread_mutex_init(&(pts->mtx_is_done), NULL);
  pts->tid = 0;
  pthread_attr_init(&pts->attr);
  pts->resources.parg_ep = pts->pep;
  pts->resources.pthread_settings = (void*)pts;
  int errn =
      pthread_create(&pts->tid, &pts->attr, pts->pep, (void*)&pts->resources);
  if (errn != 0) {
    ulog(LL_E, "cannot create thread %s, E: %s", pts->pname, strerror(errn));
  } else {
    ulog(LL_I, "thread created %s, %p", pts->pname, (void*)pts->tid);
  }
  return errn;
}

bool ut_is_running(THREAD_SETTINGS* pts) {
  bool is_running = false;
  pthread_mutex_lock(&pts->mtx_is_done);
  is_running = !pts->is_done;
  pthread_mutex_unlock(&pts->mtx_is_done);
  return is_running;
}

void ut_cancel(THREAD_SETTINGS* pts) {
  if (pts == NULL)
    return;
  pthread_mutex_lock(&pts->mtx_is_done);
  ulog(LL_I, "%p thread set is done: true", (void*)pts->tid);
  pts->is_done = true;
  pthread_mutex_unlock(&pts->mtx_is_done);
}

void ut_cancel_hard(THREAD_SETTINGS* pts) {
  if (!ut_verificate(pts)) {
    ulog(LL_E, "cannot hardly cancel thread %s, no valid", pts->pname);
    return;
  }
  pthread_cancel(pts->tid);
  ulog(LL_I, "%p thread was cancelled hardly", (void*)pts->tid);
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