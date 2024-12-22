#ifndef UTHREAD_H
#define UTHREAD_H

#include <pthread.h>
#include <stdbool.h>

typedef bool (*utf_is_running)(void*);

typedef struct _RESOURCES {
  utf_is_running pf_is_running;
  void* pthread_settings;
  void* parg_ep;
} RESOURCES;

typedef void* (*EntryPointFunc)(void*);

typedef struct _THREAD_SETTINGS {
  const char* pname;
  pthread_t tid;
  pthread_attr_t attr;
  EntryPointFunc pep;
  void* parg_ep;
  RESOURCES resources;
  volatile bool is_done;
  pthread_mutex_t mtx_is_done;
} THREAD_SETTINGS;

extern bool ut_verificate(const THREAD_SETTINGS* pts);
extern void ut_prepare_thread(THREAD_SETTINGS** ppts, const char* pname);
extern void ut_destroy(THREAD_SETTINGS* pts);
extern int ut_create_default(THREAD_SETTINGS* pts);
extern int ut_join(pthread_t tid);
extern bool ut_is_running(THREAD_SETTINGS* pts);
extern void ut_cancel(THREAD_SETTINGS* pts);
extern void ut_cancel_hard(THREAD_SETTINGS* pts);

#endif  // UTHREAD_H