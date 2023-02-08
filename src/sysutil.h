#ifndef SYSUTIL_H
#define SYSUTIL_H

#include <stdbool.h>
#include <sys/types.h>

#if defined(__cplusplus)
extern "C" {
#endif

pid_t proc_find(char const *proc_name);
void daemonize(void);
bool file_exists(char const *path);
bool is_root(void);

#if defined(__cplusplus)
}
#endif

#endif // SYSUTIL_H
