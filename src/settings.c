#include "settings.h"

#include "common.h"
#include "logging.h"
#include "src/networking.h"
#include "sysutil.h"

#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include <toml.h>

#pragma region System Settings

drive_mode_t
get_driving_mode_from_str(char *str)
{
  if (strcmp(str, "single") == 0)
    return SINGLE;
  if (strcmp(str, "tank") == 0)
    return TANK;
  if (strcmp(str, "quadtank") == 0)
    return QUADTANK;
  if (strcmp(str, "mecanum") == 0)
    return MECANUM;

  LOG_MSG(LOG_FATAL, "Invalid driving mode provided!");
  return -1; /* Just because clangd complains :P */
}

char const *paths_syssettings[] = {
  "/data/syssettings.toml",
  "/etc/syssettings.toml",
  "./syssettings.toml",
  "../syssettings.toml",
  nullptr
};

char const *
find_system_settings_path(void)
{
  i32 i = 0;

  while (paths_syssettings[i] != nullptr) {
    if (file_exists(paths_syssettings[i]))
      return paths_syssettings[i];
    i++;
  }

  return nullptr;
}

system_settings_t
load_system_settings(char const *file)
{
  char err_[200];
  char *err = err_;
  system_settings_t sys = {0};
  FILE *fp = fopen(file, "r");

  sys.driving.motor_top_left = -1;
  sys.driving.motor_top_right = -1;
  sys.driving.motor_bottom_left = -1;
  sys.driving.motor_bottom_right = -1;
#define throw_error() LOG_MSG(LOG_FATAL, "Cannot parse system configuration file: %s", err);

  if (!fp) 
    LOG_MSG(LOG_FATAL, "Cannot open system configuration file `%s`: %s", file, strerror(errno));

  sys.is_valid = false;

  toml_table_t *conf = toml_parse_file(fp, err, 200);
  fclose(fp);

  if (!conf)
    throw_error();

  BREAKABLE_SCOPE()
  {
    toml_table_t *hardware_cat = toml_table_in(conf, "Hardware");
    if (!hardware_cat)
      throw_error();

    toml_array_t *motors_arr = toml_array_in(hardware_cat, "Motors");
    if (!motors_arr)
      throw_error();

    sys.hardware.motors.cnt = toml_array_nelem(motors_arr);
    sys.hardware.motors.motors = malloc(sys.hardware.motors.cnt * sizeof(motor_t));
    if (!sys.hardware.motors.motors)
      throw_error();

    for (i32 i = 0; i < sys.hardware.motors.cnt; i++) {
      toml_table_t *motor = toml_table_at(motors_arr, i);
      toml_datum_t fault = toml_int_in(motor, "fault");
      sys.hardware.motors.motors[i].pins.fault = fault.ok ? fault.u.i : 0;
      toml_datum_t pwm = toml_int_in(motor, "pwm");
      if (!pwm.ok)
        throw_error();
      sys.hardware.motors.motors[i].pins.pwm = pwm.u.i;
      toml_datum_t enabled = toml_int_in(motor, "enabled");
      sys.hardware.motors.motors[i].pins.enabled = enabled.ok ? enabled.u.i : 0;
      toml_datum_t direction = toml_int_in(motor, "direction");
      if (!direction.ok)
        throw_error();
      sys.hardware.motors.motors[i].pins.direction = direction.u.i;
      toml_datum_t inverted = toml_bool_in(motor, "inverted");
      sys.hardware.motors.motors[i].inverted = inverted.ok ? inverted.u.b : false;
      sys.hardware.motors.motors[i].enabled = false;
    }
  }

  BREAKABLE_SCOPE()
  {
    toml_table_t *video_streaming_cat = toml_table_in(conf, "VideoStreaming");
    if (!video_streaming_cat)
      throw_error();

    toml_datum_t enabled = toml_bool_in(video_streaming_cat, "Enabled");
    if (!enabled.ok)
      throw_error();

    sys.video_streaming.enabled = enabled.u.b;

    toml_datum_t camera_device = toml_string_in(video_streaming_cat, "CameraDevice");
    if (!camera_device.ok) {
      sys.video_streaming.camera_device = nullptr;
      break;
    }

    sys.video_streaming.camera_device = trim_string_fast(camera_device.u.s);
  }

  BREAKABLE_SCOPE()
  {
    toml_table_t *driving = toml_table_in(conf, "Driving");
    if (!driving)
      throw_error();

    toml_datum_t mode = toml_string_in(driving, "Mode");
    if (!mode.ok) {
      err = "Driving.Mode not set.";
      throw_error();
    }

    /* HACK: This may or may not crash when freeing memory. */
    char *mode_str = to_lower_string(trim_string_fast(mode.u.s));
    sys.driving.mode = get_driving_mode_from_str(mode_str);

    bool is_single = strcmp(mode_str, "single") == 0;
    bool is_tank = strcmp(mode_str, "tank") == 0;

    toml_datum_t motor_top_left     = toml_int_in(driving, "MotorTopLeft");
    toml_datum_t motor_top_right    = toml_int_in(driving, "MotorTopRight");
    toml_datum_t motor_bottom_left  = toml_int_in(driving, "MotorBottomLeft");
    toml_datum_t motor_bottom_right = toml_int_in(driving, "MotorBottomRight");

    if (!motor_top_left.ok) {
      err = "Driving.MotorTopLeft not set.";
      throw_error();
    }
    sys.driving.motor_top_left = motor_top_left.u.i;
    if (is_single)
      break;

    if (!motor_top_right.ok) {
      err = "Driving.MotorTopRight not set.";
      throw_error();
    }
    sys.driving.motor_top_right = motor_top_right.u.i;
    if (is_tank)
      break;

    if (!motor_bottom_left.ok || !motor_bottom_right.ok) {
      err = "Driving.MotorBottomLeft and/or Driving.MotorBottomRight not set.";
      throw_error();
    }
    sys.driving.motor_bottom_right = motor_bottom_right.u.i;
    sys.driving.motor_bottom_left = motor_bottom_left.u.i;
  }

  sys.is_valid = true;

  return sys;
}

void
print_system_settings(system_settings_t settings)
{
  i32 i;
  LOG_MSG(LOG_DEBUG, "HARDWARE:")
  LOG_MSG(LOG_DEBUG, "  Motors:")
  for (i = 0; i < settings.hardware.motors.cnt; i++) {
    motor_t *motor = &settings.hardware.motors.motors[i];
    LOG_MSG(LOG_DEBUG, "    - fault=%d pwm=%d enabled=%d dir=%d inv=%d", motor->pins.fault, motor->pins.pwm, motor->pins.enabled, motor->pins.direction, motor->inverted);
  }

  LOG_MSG(LOG_DEBUG, "VIDEO STREAMING:")
  LOG_MSG(LOG_DEBUG, "  Enabled: %d", settings.video_streaming.enabled);
  if (settings.video_streaming.camera_device)
    LOG_MSG(LOG_DEBUG, "  Camera Device: %s", settings.video_streaming.camera_device);

  LOG_MSG(LOG_DEBUG, "DRIVING:")
  LOG_MSG(LOG_DEBUG, "  Mode: %d", settings.driving.mode);
  if (settings.driving.motor_top_left != -1)
    LOG_MSG(LOG_DEBUG, "  MotorTopLeft: %d", settings.driving.motor_top_left);
  if (settings.driving.motor_top_right != -1)
    LOG_MSG(LOG_DEBUG, "  MotorTopRight: %d", settings.driving.motor_top_right);
  if (settings.driving.motor_bottom_left != -1)
    LOG_MSG(LOG_DEBUG, "  MotorBottomLeft: %d", settings.driving.motor_bottom_left);
  if (settings.driving.motor_bottom_right != -1)
    LOG_MSG(LOG_DEBUG, "  MotorBottomRight: %d", settings.driving.motor_bottom_right);
}

char const *
drive_mode_str(drive_mode_t mode) {
  switch (mode) {
    case SINGLE: return "single";
    case TANK: return "tank";
    case QUADTANK: return "quadtank";
    case MECANUM: return "mecanum";
  }

  return "(null)";
}


#pragma endregion

#pragma region User Settings

char const *paths_user_settings[] = {
  "/data/usersettings.toml",
  "/etc/usersettings.toml",
  "./usersettings.toml",
  "../usersettings.toml",
  nullptr
};

char const *
find_user_settings_path(void)
{
  i32 i = 0;

  while (paths_user_settings[i] != nullptr) {
    if (file_exists(paths_user_settings[i]))
      return paths_user_settings[i];
    i++;
  }

  return nullptr;
}

user_settings_t
load_user_settings(char const *file)
{
  char err_[200];
  char *err = err_;
  user_settings_t user = { 0 };
  FILE *fp = fopen(file, "r");

#undef throw_error
#define throw_error() LOG_MSG(LOG_FATAL, "Cannot parse user configuration file: %s", err);

  if (!fp) 
    LOG_MSG(LOG_FATAL, "Cannot open user configuration file `%s`: %s", file, strerror(errno));

  user.is_valid = false;

  toml_table_t *conf = toml_parse_file(fp, err, 200);
  fclose(fp);

  if (!conf)
    throw_error();

  user.is_valid = true;

  BREAKABLE_SCOPE()
  {
    toml_table_t *general_cat = toml_table_in(conf, "General");
    if (!general_cat)
      return user;

    toml_datum_t robot_name = toml_string_in(general_cat, "RobotName");
    if (!robot_name.ok) {
      err = "General.RobotName not set.";
      return user;
    }

    user.general.name = robot_name.u.s;
  }

  BREAKABLE_SCOPE()
  {
    toml_table_t *conectivity_cat = toml_table_in(conf, "Conectivity");
    if (!conectivity_cat)
      return user;

    toml_array_t *wifi_networks = toml_array_in(conectivity_cat, "WiFiNetworks");
    if (!wifi_networks)
      return user;

    user.conectivity.wifi_networks.cnt = toml_array_nelem(wifi_networks);
    user.conectivity.wifi_networks.networks = calloc(1, sizeof(wifi_network_t) * user.conectivity.wifi_networks.cnt);
    if (!user.conectivity.wifi_networks.networks)
      return user;

    for (i32 i = 0; i < user.conectivity.wifi_networks.cnt; i++) {
      wifi_network_t *current = &user.conectivity.wifi_networks.networks[i];
      toml_table_t *network = toml_table_at(wifi_networks, i);
      toml_datum_t ssid = toml_string_in(network, "ssid");
      if (!ssid.ok) {
        err = "Conectivity.WiFiNetworks.*.ssid not set.";
        return user;
      }
      current->ssid = ssid.u.s;

      toml_datum_t password = toml_string_in(network, "password");
      if (!password.ok) {
        err = "Conectivity.WiFiNetworks.*.password not set.";
        current->password = nullptr;
        continue;
      }
      current->password = password.u.s;
    }
  }

  return user;
}

void
print_user_settings(user_settings_t settings)
{
  i32 i;
  LOG_MSG(LOG_DEBUG, "GENERAL:")
  LOG_MSG(LOG_DEBUG, "  RobotName: `%s`", settings.general.name);

  LOG_MSG(LOG_DEBUG, "CONECTIVITY:")
  LOG_MSG(LOG_DEBUG, "  WiFiNetworks:");
  for (i = 0; i < settings.conectivity.wifi_networks.cnt; i++) {
    wifi_network_t current = settings.conectivity.wifi_networks.networks[i];
    LOG_MSG(LOG_DEBUG, "    ssid: `%s` password: `%s`", current.ssid, current.password);
  }
}

#pragma endregion

