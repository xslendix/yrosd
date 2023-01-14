#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <pthread.h>

#include "common.h"

pthread_t *start_main_server(void);

#if defined(__cplusplus)
}
#endif

