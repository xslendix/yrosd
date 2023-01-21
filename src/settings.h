#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>

#include "common.h"
#include "types.h"
#include "networking.h"

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
        motor_bottom_left, motor_bottom_right;
  } driving;

  bool is_valid;
} system_settings_t;

char const *find_system_settings_path(void);
system_settings_t load_system_settings(char const *file);
void print_system_settings(system_settings_t settings);

char const *drive_mode_str(drive_mode_t mode);

typedef struct user_settings {
  struct {
    char *name;
  } general;
  struct {
    wifi_network_list_t wifi_networks;
  } conectivity;
  
  bool is_valid;
} user_settings_t;

char const *find_user_settings_path(void);
user_settings_t load_user_settings(char const *file);
void print_user_settings(user_settings_t settings);

#if defined(__cplusplus)
}
#endif

#endif // SETTINGS_H
