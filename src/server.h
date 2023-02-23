#ifndef SERVER_H
#define SERVER_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "common.h"

#include <pthread.h>

pthread_t *start_main_server(void);

typedef enum c_mode {
  MODE_AUTH,
  MODE_COMMAND,
} c_mode_t;

#if defined(__cplusplus)
}
#endif

#endif // SERVER_H
