#pragma once

#include <stdio.h>
#if defined(__cplusplus)
extern "C" {
#endif

typedef enum log_level {
  LOG_FATAL,
  LOG_ERROR,
  LOG_WARNING,
  LOG_INFO,
  LOG_DEBUG,
  LOG_FIXME,
} log_level_t;

typedef struct logging {
  char *log_file;
  FILE *fp;
  log_level_t current_level;
} logging_t;

extern logging_t *DEFAULT_LOGGER;

logging_t *init_logging();
void log_msg(logging_t *logger, log_level_t const level, char const *format, ...);
#define LOG_MSG(level, format, ...) log_msg(DEFAULT_LOGGER, level, format, ##__VA_ARGS__);
void free_logging(logging_t *logger);

#if defined(__cplusplus)
}
#endif

