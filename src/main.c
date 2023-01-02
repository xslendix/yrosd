#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "logging.h"
#include "sysutil.h"
#include "settings.h"

#include "pigpiod_if.h"

system_settings_t system_settings;

void
clean_quit(void)
{
  free_system_settings(system_settings);
  pigpio_stop();
}

i32
main(i32 argc, char **argv)
{
  i32 i;

  LOG_MSG(LOG_INFO, "REAL %d", 123);

  /* FIXME: Make this work. */
  // if (proc_find("yrosd", argv[0]) == -1)
  //   LOG_MSG(LOG_FATAL, "Already running!");

  for (i = 1; i < argc; i++)
    if (strcmp(argv[i], "-D") == 0)
      if (!fork())
        return EXIT_SUCCESS;

  pigpio_start(NULL, NULL);

  system_settings = load_system_settings("/data/syssettings.toml");
  

  clean_quit();

  return EXIT_SUCCESS;
}

