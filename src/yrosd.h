#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include "settings.h"

#include <NetworkManager.h>

typedef struct app {
  system_settings_t system_settings;
  user_settings_t user_settings;
  NMClient *client;
} app_t;

extern app_t app;

#if defined(__cplusplus)
}
#endif

