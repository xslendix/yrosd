#pragma once

#include <stdbool.h>

#include "common.h"
#include "types.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum drive_mode {
  SINGLE,
  TANK,
  QUADTANK,
  MECANUM,
} drive_mode_t;

typedef struct system_settings {
  struct {
    motor_list_t motors;
  } hardware;
  struct {
    bool enabled;
    char *camera_device;
  } video_streaming;
  struct {
    drive_mode_t mode;
    i32 motor_top_left, motor_top_right,
        motor_right_left, motor_right_right;
  } driving;
} system_settings_t;

system_settings_t load_system_settings(char *file);

#if defined(__cplusplus)
}
#endif

