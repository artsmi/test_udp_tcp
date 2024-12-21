#include "../include/logger.h"
#include <sys/types.h>
#define _DECLARE_GETTID
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


void ulog(LL log_level, const char* pmessage, ...) {
  va_list args;
  va_start(args, pmessage);

  const char* ptag = NULL;
  switch (log_level) {
    case LL_D:
      ptag = "DEBUG";
      break;
    case LL_I:
      ptag = "INFO";
      break;
    case LL_W:
      ptag = "WARN";
      break;
    case LL_E:
    default:
      ptag = "ERR";
      break;
  }

  time_t now;
  time(&now);
  char* pstrdate = ctime(&now);
  pstrdate[strlen(pstrdate) - 1] = '\0';

  pid_t tid = gettid();
  printf("%s [0x%x][%s]:", pstrdate, tid, ptag);
  vprintf(pmessage, args);
  printf("\n");

  va_end(args);
}
