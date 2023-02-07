#include "sysutil.h"

#include "common.h"
#include "yrosd.h"

#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_LEN (512)

pid_t
proc_find(char const *proc_name)
{
  int pid = -1;
  char cmdPath[MAX_LEN];
  char cmdLine[MAX_LEN];
  char *pos;
  FILE *cmdFile;

  DIR *dp = opendir("/proc");
  if (dp != NULL) {
    struct dirent *dirp;
    while (pid < 0 && (dirp = readdir(dp))) {
      int id = atoi(dirp->d_name);
      if (id > 0) {
        sprintf(cmdPath, "/proc/%s/cmdline", dirp->d_name);
        cmdFile = fopen(cmdPath, "r");
        fgets(cmdLine, MAX_LEN, cmdFile);
        fclose(cmdFile);
        if (strlen(cmdLine) > 0) {
          pos = strchr(cmdLine, '\0');
          if (pos != NULL)
            *pos = '\0';
          pos = strrchr(cmdLine, '/');
          if (pos != NULL)
            strcpy(cmdLine, pos + 1);
          if (strcmp(proc_name, cmdLine) == 0)
            pid = id;
        }
      }
    }
  }

  closedir(dp);

  return pid;
}

void
daemonize()
{
  pid_t pid;

  pid = fork();
  if (pid < 0)
    exit(EXIT_FAILURE);
  if (pid > 0)
    exit(EXIT_SUCCESS);
  if (setsid() < 0)
    exit(EXIT_FAILURE);

  initialise_signals();

  pid = fork();
  if (pid < 0)
    exit(EXIT_FAILURE);
  if (pid > 0)
    exit(EXIT_SUCCESS);
}

bool
file_exists(char const *path)
{
  return access(path, F_OK) == 0;
}
