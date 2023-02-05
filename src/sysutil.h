#ifndef SYSUTIL_H
#define SYSUTIL_H

#include <stdbool.h>
#include <sys/types.h>

#if defined(__cplusplus)
extern "C" {
#endif

pid_t proc_find(char const *proc_name);
void daemonize();
bool file_exists(char const *path);

#if defined(__cplusplus)
}
#endif

#endif // SYSUTIL_H
