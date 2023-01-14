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

#if defined(__cplusplus)
}
#endif

#endif // SERVER_H

