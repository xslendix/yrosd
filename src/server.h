#ifndef SERVER_H
#define SERVER_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <pthread.h>

#include "common.h"

pthread_t *start_main_server(void);

typedef enum c_mode {
  MODE_AUTH,
  MODE_SETUP,
  MODE_COMMAND,
} c_mode_t;

char const *c_mode_name(c_mode_t mode) {
  switch (mode) {
    case MODE_AUTH:    return "Auth";
    case MODE_SETUP:   return "Setup";
    case MODE_COMMAND: return "Command";
  }

  return "(Unknown)";
}

#if defined(__cplusplus)
}
#endif

#endif // SERVER_H

