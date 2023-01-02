#include "logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

logging_t *DEFAULT_LOGGER = NULL;

logging_t *
init_logging()
{
  logging_t *logger;
  logger = malloc(sizeof(logging_t));
  logger->log_file = NULL;
  logger->fp = NULL;
  logger->current_level = LOG_DEBUG;
  return logger;
}

char const *
get_log_level_name(log_level_t level)
{
  switch (level)
  {
  case LOG_FATAL:   return "Fatal";
  case LOG_ERROR:   return "Error";
  case LOG_WARNING: return "Warning";
  case LOG_INFO:    return "Info";
  case LOG_DEBUG:   return "Debug";
  }

  return "UNKNOWN";
}

void
log_msg(logging_t *logger, log_level_t const level, char const *format, ...)
{
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  FILE *fp = logger->fp;
  va_list ap;
  va_start(ap, format);

#define LOG()                                                                  \
  {                                                                            \
    fprintf(fp, "[%02d-%02d-%d %d:%d:%d] %s: ", tm.tm_mday, tm.tm_mon + 1,     \
            tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec,               \
            get_log_level_name(level));                                        \
    vfprintf(fp, format, ap);                                                  \
    fputc('\n', fp);                                                           \
  }

  if (fp)
    LOG();

  fp = stdout;
  if (level <= logger->current_level)
    LOG();

  if (level == LOG_FATAL)
    abort();
}

void
free_logging(logging_t *logger)
{
  if (logger)
    free(logger);
}

void __attribute__((constructor))
init_library_hidden()
{
  DEFAULT_LOGGER = init_logging();
}

void __attribute__((destructor))
free_library_hidden()
{
  free_logging(DEFAULT_LOGGER);
}