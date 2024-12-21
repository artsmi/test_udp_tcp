#ifndef LOGGER_H
#define LOGGER_H

typedef enum { LL_E = 0, LL_W = 1, LL_I = 2, LL_D = 3 } LL;

extern void ulog(LL log_level, const char* message, ...);

#endif  // LOGGER_H