#include "../include/thread_mgr.h"
#include <logger.h>

void thm_prepare(THREAD_MANAGER* pmgr) {
  if (pmgr == NULL) {
    return;
  }
  memset(&pmgr->threads, 0, sizeof(pmgr->threads));
  pmgr->nthreads = 0;
}

THREAD_SETTINGS* thm_add_thread(THREAD_MANAGER* pmgr,
                                const char* pname,
                                EntryPointFunc pfunc,
                                void* pdata) {
  if (pmgr == NULL) {
    return NULL;
  }
  THREAD_SETTINGS** ppthread = NULL;
  bool is_done = false;
  for (int i = 0; !is_done && i < THM_MAX_THREAD_SUPPORT; ++i) {
    ppthread = pmgr->threads + i;
    if (*ppthread == NULL) {
      is_done = true;
    }
  }
  ut_prepare_thread(ppthread, pname);
  THREAD_SETTINGS* pthread = *ppthread;
  pthread->pep = pfunc;
  pthread->parg_ep = pdata;
  if (0 != ut_create_default(pthread)) {
    pthread = NULL;
  } else {
    ++(pmgr->nthreads);
  }
  return pthread;
}

void thm_stop_destroy_all(THREAD_MANAGER* pmgr) {
  if (pmgr == NULL) {
    ulog(LL_E, "thm_stop_all bad pointer");
    return;
  }
  THREAD_SETTINGS** ppthread = NULL;
  size_t ths_size = pmgr->nthreads;
  for (int i = 0; i < ths_size; ++i) {
    ppthread = pmgr->threads + i;
    THREAD_SETTINGS* pthread = *ppthread;
    if (pthread == NULL) {
      ulog(LL_E, "thm_stop_all counter overhead: %d", i);
    } else {
      ut_cancel(pthread);
      ut_join(pthread->tid);
      ut_destroy(pthread);
    }
  }
}