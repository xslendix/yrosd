#include "src/motor_controller.h"
#include "src/networking.h"
#include "yrosd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "logging.h"
#include "sysutil.h"
#include "settings.h"
#include "server.h"

#include "pigpiod_if2.h"

app_t app;

void
clean_quit(void)
{
  /* TODO: Free the memory! */
  //free_system_settings(system_settings);

  pigpio_stop(app.pi);
}

i32
main(i32 argc, char **argv)
{
  i32 i;

  atexit(clean_quit);

  /* FIXME: Make this work. */
  // if (proc_find("yrosd", argv[0]) == -1)
  //   LOG_MSG(LOG_FATAL, "Already running!");

  LOG_MSG(LOG_INFO, "Starting NetworkManager.");
  app.client = nm_client_new(nullptr, nullptr);
  if (!app.client)
    LOG_MSG(LOG_FATAL, "Cannot connect to NetworkManager!");

  for (i = 1; i < argc; i++)
    if (strcmp(argv[i], "-D") == 0)
      if (!fork())
        return EXIT_SUCCESS;

  LOG_MSG(LOG_INFO, "Starting pigpio connection.");
  if ((app.pi = pigpio_start(nullptr, nullptr)) < 0)
    LOG_MSG(LOG_FATAL, "Cannot initialize pigpio connection.");

  if (IS_FAIL(motor_controller_init(&app.motor_controller)))
    LOG_MSG(LOG_FATAL, "Cannot motor controller.");

  app.running_port = random_u16(8000, 10000);
  app.version = "23.1";

  LOG_MSG(LOG_INFO, "Loading system configuration file.");
  char const *syssettings_path = find_system_settings_path();
  if (!syssettings_path)
    LOG_MSG(LOG_FATAL, "Cannot find path of `syssettings.toml`!");

  app.system_settings = load_system_settings(syssettings_path);
  print_system_settings(app.system_settings);

  LOG_MSG(LOG_INFO, "Loading user configuration file.");
  char const *user_settings_path = find_user_settings_path();
  app.user_settings = load_user_settings(user_settings_path);
  if (!app.user_settings.is_valid) {
    LOG_MSG(LOG_FIXME, "Implement SETUP MODE.");
    LOG_MSG(LOG_FATAL, "Cannot proceed further, invalid user configuration.");
  }
  print_user_settings(app.user_settings);

  pthread_t *thread = start_broadcasting();
  pthread_t *thread_auth = start_main_server();
  pthread_join(*thread, nullptr);
  pthread_join(*thread_auth, nullptr);

  return EXIT_SUCCESS;
}

