#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#include "common.h"

typedef struct motor {
  struct {
    u8 fault, pwm, enabled, direction;
  } pins;
  bool inverted, enabled;
} motor_t;

typedef struct motor_list {
  motor_t *motors;
  i32 cnt;
} motor_list_t;

#endif // TYPES_H
