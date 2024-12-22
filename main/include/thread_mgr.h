#ifndef THREAD_MGR_H
#define THREAD_MGR_H

#include <string.h>
#include <uthread.h>

#define THM_MAX_THREAD_SUPPORT (30)

typedef struct _THREAD_MANAGER {
  THREAD_SETTINGS* threads[THM_MAX_THREAD_SUPPORT];
  size_t nthreads;
} THREAD_MANAGER;

extern void thm_prepare(THREAD_MANAGER* pmgr);
extern THREAD_SETTINGS* thm_add_thread(THREAD_MANAGER* pmgr,
                                       const char* pname,
                                       EntryPointFunc pfunc,
                                       void* pdata);

extern void thm_stop_destroy_all(THREAD_MANAGER* pmgr);
extern void thm_wait_all(THREAD_MANAGER* pmgr);
extern void thm_cancell_all(THREAD_MANAGER* pmgr);
extern void thm_destroy_all(THREAD_MANAGER* pmgr);
extern void thm_emergency_all(THREAD_MANAGER* pmgr);

#endif  // THREAD_MGR_H