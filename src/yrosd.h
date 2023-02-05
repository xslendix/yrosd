#ifndef YROSD_H
#define YROSD_H

#include "src/motor_controller.h"
#if defined(__cplusplus)
extern "C" {
#endif

#define VERSION "v0.1"
#define PROTOCOL_VERSION "v1.0"

#include "settings.h"
#include "networking.h"

#include <NetworkManager.h>

typedef struct app {
  system_settings_t system_settings;
  user_settings_t user_settings;
  NMClient *client;
  u16 running_port;
  ipv4_addr_t running_ip;
  char const *version;
  i32 pi;
  motor_controller_t motor_controller;
} app_t;

extern app_t app;

#if defined(__cplusplus)
}
#endif

#endif // YROSD_H
