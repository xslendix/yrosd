#include "common.h"

#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

char *
trim_string_fast(char *str)
{
  /* This function trims a given string without allocating new memory.
   * However, it does set a \0 at the end and advances the pointer accordingly.*/
  int i, len;

  if (*str == '\0')
    return str;

  for (;;) {
    if (*str == '\0')
      return str;

    if (!isblank(*str))
      break;

    str++;
  }

  len = strlen(str);
  for (i = len; i > 0; i--)
    if (!isblank(str[i - 1]))
      break;

  str[i] = '\0';

  return str;
}

char *
to_lower_string(char *str)
{
  char *ret = str;

  while (*str) {
    *str = tolower(*str);
    str++;
  }

  return ret;
}

u16
random_u16(u16 min, u16 max)
{
  srand(time(nullptr));
  return (rand() % (max - min + 1)) + min;
}
