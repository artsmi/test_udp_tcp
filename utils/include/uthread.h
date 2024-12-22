#ifndef UTHREAD_H
#define UTHREAD_H

#include <pthread.h>
#include <stdbool.h>

typedef void (*EmergencyFunc)(void* presources);

typedef struct _RESOURCES {
  void* pthread_settings;
  void* parg_ep;
  volatile EmergencyFunc pf_emergency;
  volatile int data_value;
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
} THREAD_SETTINGS;

extern bool ut_verificate(const THREAD_SETTINGS* pts);
extern void ut_prepare_thread(THREAD_SETTINGS** ppts, const char* pname);
extern void ut_destroy(THREAD_SETTINGS* pts);
extern int ut_create_default(THREAD_SETTINGS* pts);
extern int ut_join(pthread_t tid);
extern void ut_cancel(THREAD_SETTINGS* pts);
extern void ut_cancel_hard(THREAD_SETTINGS* pts);
extern void ut_emergency(THREAD_SETTINGS* pts);

extern bool utr_is_running(RESOURCES* pres);
extern void utr_setup_emergency_function(RESOURCES* pres, EmergencyFunc pf);

#endif  // UTHREAD_H