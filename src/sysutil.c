#include "sysutil.h"

#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

/* https://stackoverflow.com/a/6898456 */
pid_t
proc_find(char const *name, char const *current)
{
  DIR *dir;
  struct dirent *ent;
  char *endptr;
  char buf[512];

  if (!(dir = opendir("/proc"))) {
    perror("can't open /proc");
    return -1;
  }

  while ((ent = readdir(dir)) != nullptr) {
    /* if endptr is not a null character, the directory is not
     * entirely numeric, so ignore it */
    i64 lpid = strtol(ent->d_name, &endptr, 10);
    if (*endptr != '\0') {
      continue;
    }

    /* try to open the cmdline file */
    snprintf(buf, sizeof(buf), "/proc/%lld/cmdline", lpid);
    FILE *fp = fopen(buf, "r");

    if (fp) {
      if (fgets(buf, sizeof(buf), fp) != nullptr) {
        /* check the first token in the file, the program name */
        char *first = strtok(buf, " ");
        if (!strcmp(first, name) || strcmp(first, current)) {
          fclose(fp);
          closedir(dir);
          return (pid_t)lpid;
        }
      }
      fclose(fp);
    }
  }

  closedir(dir);
  return -1;
}

void
daemonize()
{
  pid_t pid;

  /* Fork off the parent process */
  pid = fork();

  /* An error occurred */
  if (pid < 0)
    exit(EXIT_FAILURE);

  /* Success: Let the parent terminate */
  if (pid > 0)
    exit(EXIT_SUCCESS);

  /* On success: The child process becomes session leader */
  if (setsid() < 0)
    exit(EXIT_FAILURE);
}

bool file_exists(char const *path)
{
  return access(path, F_OK) == 0;
}
