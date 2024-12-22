#ifndef UTHREAD_H
#define UTHREAD_H

#include <pthread.h>
#include <stdbool.h>

typedef void* (*EntryPointFunc)(void*);

typedef struct _THREAD_SETTINGS {
  const char* pname;
  pthread_t tid;
  pthread_attr_t attr;
  EntryPointFunc pep;
  void* parg_ep;
} THREAD_SETTINGS;

extern bool ut_verificate(const THREAD_SETTINGS* pts);
extern void ut_prepare_thread(THREAD_SETTINGS** ppts, const char* pname);
extern void ut_destroy(THREAD_SETTINGS* pts);
extern int ut_create_default(THREAD_SETTINGS* pts);
extern int ut_join(pthread_t tid);

#endif  // UTHREAD_H