#include "logging.h"

#include "common.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

logging_t *DEFAULT_LOGGER = nullptr;

void
set_level_logging_from_str(logging_t *logger, char *level)
{
  if (!level)
    return;

  if (!logger)
    logger = DEFAULT_LOGGER;

  if (!logger)
    init_logging();

  to_lower_string(level);
  trim_string_fast(level);

  if (strcmp(level, "debug") == 0)
    logger->current_level = LOG_DEBUG;
  else if (strcmp(level, "fixme") == 0)
    logger->current_level = LOG_FIXME;
  else if (strcmp(level, "info") == 0)
    logger->current_level = LOG_INFO;
  else if (strcmp(level, "warning") == 0)
    logger->current_level = LOG_WARNING;
  else if (strcmp(level, "error") == 0)
    logger->current_level = LOG_ERROR;
  else if (strcmp(level, "fatal") == 0)
    logger->current_level = LOG_FATAL;
  else
    LOG_MSG(LOG_FATAL, "Invalid logging level!");
}

logging_t *
init_logging()
{
  logging_t *logger;
  logger                = malloc(sizeof(logging_t));
  logger->log_file      = nullptr;
  logger->fp            = nullptr;
  logger->current_level = LOG_DEBUG;

  char *level = getenv("LOG_LEVEL");
  if (!level)
    return logger;

  set_level_logging_from_str(logger, level);

  return logger;
}

char const *
get_log_level_name(log_level_t level)
{
  switch (level) {
  case LOG_FATAL: return "\e[31mFatal\e[0m";
  case LOG_ERROR: return "\e[35mError\e[0m";
  case LOG_WARNING: return "\e[33mWarning\e[0m";
  case LOG_INFO: return "\e[36mInfo\e[0m";
  case LOG_DEBUG: return "Debug";
  case LOG_FIXME: return "Fixme";
  }

  return "UNKNOWN";
}

void
log_msg(logging_t *logger, log_level_t const level, char const *format, ...)
{
  time_t t     = time(nullptr);
  struct tm tm = *localtime(&t);
  FILE *fp     = logger->fp;
  va_list ap;
  va_start(ap, format);

#define LOG()                                                                  \
  {                                                                            \
    fprintf(fp, "[%02d-%02d-%d %02d:%02d:%02d] %s: ", tm.tm_mday,              \
            tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min,           \
            tm.tm_sec, get_log_level_name(level));                             \
    vfprintf(fp, format, ap);                                                  \
    fputc('\n', fp);                                                           \
  }

  if (fp)
    LOG();

  fp = stdout;
  if (level <= logger->current_level)
    LOG();

  if (level == LOG_FATAL)
    exit(EXIT_FAILURE);
}

void
free_logging(logging_t *logger)
{
  if (logger)
    free(logger);
}

void __attribute__((constructor)) init_library_hidden()
{
  DEFAULT_LOGGER = init_logging();
}

void __attribute__((destructor)) free_library_hidden()
{
  free_logging(DEFAULT_LOGGER);
}
