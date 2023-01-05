#pragma once

#include <stdbool.h>
#include <sys/types.h>

#if defined(__cplusplus)
extern "C" {
#endif

pid_t proc_find(char const *name, char const *current);
void daemonize();
bool file_exists(char const *path);

#if defined(__cplusplus)
}
#endif

