#include <string.h>
#include <errno.h>

#include "settings.h"
#include "logging.h"

#include <toml.h>

system_settings_t
load_system_settings(char *file)
{
  FILE *fp = fopen(file, "r");
  if (!fp) 
    LOG_MSG(LOG_FATAL, "Cannot open system configuration file `%s`: %s", file, strerror(errno));

  system_settings_t sys;


  fclose(fp);

  return sys;
}

