#include "common.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

char *
trim_string_fast(char *str)
{
  char *end;

  if (*str == '\0')
    return str;

  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)
    return str;

  end = str + strlen(str) - 1;
  while(end > str && (isspace(*end) || isblank(*end))) end--;

  end[1] = '\0';

  return str;
}

char *
to_lower_string(char *str)
{
  if (str == nullptr)
    return nullptr;

  char *ret = str;

  while (*str) {
    *str = tolower(*str);
    str++;
  }

  return ret;
}

char format_buf[1024];

char const *
text_format(char *format, ...)
{
  va_list lst;
  va_start(lst, format);
  vsnprintf(format_buf, sizeof(format_buf), format, lst);
  va_end(lst);

  return format_buf;
}

u16
random_u16(u16 min, u16 max)
{
  srand(time(nullptr));
  return (rand() % (max - min + 1)) + min;
}

bool
is_str_alphanumeric(char *str)
{
  while (*str) {
    if (*str > 127 || *str < 0)
      return false;
    str++;
  }
  return true;
}

