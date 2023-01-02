#pragma once

#include <sys/types.h>

#if defined(__cplusplus)
extern "C" {
#endif

pid_t proc_find(char const *name, char const *current);
void daemonize();

#if defined(__cplusplus)
}
#endif

