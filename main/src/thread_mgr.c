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

typedef enum {
  THM_OP_WAIT = 0,
  THM_OP_CANCEL = 1,
  THM_OP_DESTROY = 2,
  THM_OP_EMERGENCY = 3
} THM_OPS;

void _thm_do_all(THREAD_MANAGER* pmgr, THM_OPS code) {
  if (pmgr == NULL) {
    ulog(LL_E, "thm_do_all bad pointer");
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
      switch (code) {
        case THM_OP_WAIT:
          ut_join(pthread);
          break;
        case THM_OP_CANCEL:
          ut_cancel(pthread);
          break;
        case THM_OP_EMERGENCY:
          ut_emergency(pthread);
          break;
        case THM_OP_DESTROY:
          ut_destroy(pthread);
          *ppthread = NULL;
          --(pmgr->nthreads);
          break;
        default:
          break;
      }
    }
  }
}

void thm_stop_destroy_all(THREAD_MANAGER* pmgr) {
  _thm_do_all(pmgr, THM_OP_CANCEL);
  _thm_do_all(pmgr, THM_OP_WAIT);
  _thm_do_all(pmgr, THM_OP_DESTROY);
}

void thm_wait_all(THREAD_MANAGER* pmgr) {
  _thm_do_all(pmgr, THM_OP_WAIT);
}

void thm_cancell_all(THREAD_MANAGER* pmgr) {
  _thm_do_all(pmgr, THM_OP_CANCEL);
}

void thm_destroy_all(THREAD_MANAGER* pmgr) {
  _thm_do_all(pmgr, THM_OP_DESTROY);
}

void thm_emergency_all(THREAD_MANAGER* pmgr) {
  _thm_do_all(pmgr, THM_OP_EMERGENCY);
}