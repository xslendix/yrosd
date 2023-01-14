#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#include "common.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct motor {
  u8 fault, pwm, enabled, direction;
  bool inverted;
} motor_t;

typedef struct motor_list {
  motor_t *motors;
  i32 cnt;
} motor_list_t;

#if defined(__cplusplus)
}
#endif

#endif // TYPES_H
