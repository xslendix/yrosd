#include "server.h"

#include "common.h"
#include "logging.h"
#include "motor_controller.h"
#include "pigpiod_if2.h"
#include "sysutil.h"
#include "yrosd.h"

#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <openssl/sha.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

pthread_t thread_main;

typedef struct client_data {
  c_mode_t mode;
} client_data_t;

char const *paths_password_file[] = { "/data/psk", "/etc/psk", "./psk",
                                      "../psk", nullptr };

char const *
find_password_file_path(void)
{
  i32 i = 0;

  while (paths_password_file[i] != nullptr) {
    if (file_exists(paths_password_file[i]))
      return paths_password_file[i];
    i++;
  }

  return nullptr;
}

bool
check_password_valid(char *psk)
{
  trim_string_fast(psk);

  char hash[SHA256_DIGEST_LENGTH];

  /* We compare the password with the hashed stored version. */
  SHA256((u8 *) psk, strlen(psk), (u8 *) hash);

  char const *file_path = find_password_file_path();
  if (file_path == nullptr)
    LOG_MSG(LOG_FATAL, "Auth: Cannot find password file!");

  FILE *fp = fopen(file_path, "rb");
  fseek(fp, 0, SEEK_END);
  size_t buf_len = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  if (buf_len != SHA256_DIGEST_LENGTH)
    LOG_MSG(LOG_FATAL, "Auth: Invalid password file!");

  u8 *buffer = malloc(buf_len + 1);
  fread(buffer, buf_len, 1, fp);
  fclose(fp);

  buffer[buf_len] = 0;

  i32 status = memcmp(hash, buffer, SHA256_DIGEST_LENGTH) == 0;
  free(buffer);
  return status == 1;
}

#define SEND_MSG(sock, msg)                                                    \
  BREAKABLE_SCOPE()                                                            \
  {                                                                            \
    char const *_msg = msg;                                                    \
    if ((ret = send(sock, _msg, strlen(_msg), 0)) != strlen(_msg)) {           \
      LOG_MSG(LOG_ERROR, "Main server: Failed sending message: %s",            \
              strerror(errno));                                                \
      break;                                                                   \
    }                                                                          \
  }

/* clang-format off */
char const *SETUP_OK              = (char[]) { 0x00, '\r', '\n', '\0' }; /* Data on the server has been processed correctly. */
char const *SETUP_CLIENT_OK       = (char[]) { 0x01, '\r', '\n', '\0' }; /* Data on the client has been processed correctly. */
char const *SETUP_ERR_INVALID     = (char[]) { 0x02, '\r', '\n', '\0' }; /* Invalid configuration provided. */
char const *AUTH_PASS_FAIL        = (char[]) { 0x10, '\r', '\n', '\0' }; /* Authentication failed. Please try again. */
char const *AUTH_PASS_OK          = (char[]) { 0x10, '\r', '\n', '\0' }; /* Authentication succeeded. */
char const *COMMAND_OK            = (char[]) { 0x20, '\r', '\n', '\0' }; /* Command executed successfully. */
char const *COMMAND_ERR_UNK       = (char[]) { 0x21, '\r', '\n', '\0' }; /* An unknown error occured. */
char const *COMMAND_ERR_ARGC      = (char[]) { 0x22, '\r', '\n', '\0' }; /* Not enough arguments. */
char const *COMMAND_ERR_INV_ARG   = (char[]) { 0x23, '\r', '\n', '\0' }; /* Invalid argument(s). */
char const *COMMAND_ERR_NOT_FOUND = (char[]) { 0x24, '\r', '\n', '\0' }; /* Cannot find data/target specified. */
char const *COMMAND_ERR_CFG       = (char[]) { 0x35, '\r', '\n', '\0' }; /* Command not configured. */
/* clang-format on */

void
execute_auth(i32 i, char *msg, i32 sock, client_data_t *data, i32 argc,
             char *argv[])
{
  i32 ret;
  if (check_password_valid(msg)) {
    LOG_MSG(LOG_INFO, "Main server: New client logged in successfully!");
    SEND_MSG(sock, AUTH_PASS_OK);
    data->mode = MODE_COMMAND;
  } else {
    LOG_MSG(LOG_INFO, "Main server: New client failed to log in! Either a bad "
                      "configuration on client side or a possible attack.");
    SEND_MSG(sock, AUTH_PASS_FAIL);
  }
}

typedef enum gpio_action {
  ACTION_ANALOGUE_WRITE,
  ACTION_DIGITAL_WRITE,
} gpio_action_t;

typedef enum motor_action {
  ACTION_MOTOR_SET,
  ACTION_MOTOR_STOP,
  ACTION_MOTOR_DISABLE,
  ACTION_MOTOR_ENABLE,
} motor_action_t;

void
execute_command(i32 i, char *msg, i32 sock, client_data_t *data, i32 argc,
                char *argv[])
{
  i32 ret;

  if (argc < 1) {
    send(sock, COMMAND_ERR_ARGC, strlen(COMMAND_ERR_ARGC), 0);
    return;
  }

  errno = 0;

  to_lower_string(argv[0]);
  LOG_MSG(LOG_DEBUG, "Command: `%s` by socket %d", argv[0], sock);
  if (!strcmp(argv[0], "info")) {
    char features[50] = "Features=[";
    if (app.system_settings.video_streaming.enabled)
      snprintf(features + 10, sizeof(features) - 10, "'VideoStreaming']");
    else {
      features[10] = ']';
      features[11] = '\0';
    }
    SEND_MSG(sock, text_format("%s\r\n"
                               "RobotName='%s'\r\n"
                               "DrivingMode='%s'\r\n%s",
                               features, app.user_settings.general.name,
                               drive_mode_str(app.system_settings.driving.mode),
                               COMMAND_OK));
  } else if (!strcmp(argv[0], "gpio")) {
    if (argc < 3) {
      LOG_MSG(LOG_DEBUG, "Main server: GPIO: Not enough arguments provided.");
      SEND_MSG(sock, COMMAND_ERR_ARGC);
      return;
    }

    i32 pin = strtol(argv[1], NULL, 0);
    if (errno == EINVAL || errno == ERANGE) {
      LOG_MSG(LOG_ERROR, "Main server: GPIO: Invalid GPIO pin specified.");
      SEND_MSG(sock, COMMAND_ERR_INV_ARG);
      return;
    }

    char *action_str = to_lower_string(argv[2]);
    gpio_action_t action;
    if (!strcmp(action_str, "analogue_write") || !strcmp(action_str, "aw"))
      action = ACTION_ANALOGUE_WRITE;
    else if (!strcmp(action_str, "digital_write") || !strcmp(action_str, "dw"))
      action = ACTION_DIGITAL_WRITE;
    else {
      LOG_MSG(LOG_ERROR, "Main server: GPIO: Invalid GPIO action specified.");
      SEND_MSG(sock, COMMAND_ERR_INV_ARG);
      return;
    }

    argv += 3;
    argc -= 3;
    switch (action) {
    case ACTION_ANALOGUE_WRITE:
    {
      SEND_MSG(sock, COMMAND_ERR_CFG);
      return;

      if (argc < 1) {
        LOG_MSG(LOG_ERROR, "Main server: GPIO: No GPIO value specified.");
        SEND_MSG(sock, COMMAND_ERR_INV_ARG);
        return;
      }

      i32 value = strtol(argv[0], NULL, 0);
      if (errno == EINVAL || errno == ERANGE) {
        LOG_MSG(LOG_ERROR,
                "Main server: GPIO: GPIO value specified is not a number.");
        SEND_MSG(sock, COMMAND_ERR_INV_ARG);
        return;
      }

      value = CLAMP(value, 0, 255);

      /* FIXME: Implement. */
    }
    case ACTION_DIGITAL_WRITE:
    {
      if (argc < 1) {
        LOG_MSG(LOG_ERROR, "Main server: GPIO: No GPIO value specified.");
        SEND_MSG(sock, COMMAND_ERR_INV_ARG);
      }

      i32 value = strtol(argv[0], NULL, 0);
      if (errno == EINVAL || errno == ERANGE) {
        LOG_MSG(LOG_ERROR,
                "Main server: GPIO: GPIO value specified is not a number.");
        SEND_MSG(sock, COMMAND_ERR_INV_ARG);
        return;
      }

      value = CLAMP(value, 0, 1);

      gpio_write(app.pi, pin, value);
    } break;
    }
  } else if (!strcmp(argv[0], "motor") || !strcmp(argv[0], "m")) {
    if (argc < 3) {
      LOG_MSG(LOG_DEBUG, "Main server: MOTOR: Not enough arguments provided.");
      SEND_MSG(sock, COMMAND_ERR_ARGC);
      return;
    }

    i32 motor_id = strtol(argv[1], NULL, 0);
    if (errno == EINVAL || errno == ERANGE ||
        motor_id >= app.system_settings.hardware.motors.cnt) {
      LOG_MSG(LOG_ERROR, "Main server: MOTOR: Invalid motor ID specified.");
      SEND_MSG(sock, COMMAND_ERR_INV_ARG);
      return;
    }

    char *action_str = to_lower_string(argv[2]);
    motor_action_t action;
    if (!strcmp(action_str, "set") || !strcmp(action_str, "s"))
      action = ACTION_MOTOR_SET;
    else if (!strcmp(action_str, "stop") || !strcmp(action_str, "t"))
      action = ACTION_MOTOR_STOP;
    else if (!strcmp(action_str, "disable"))
      action = ACTION_MOTOR_DISABLE;
    else if (!strcmp(action_str, "enable"))
      action = ACTION_MOTOR_ENABLE;
    else {
      LOG_MSG(LOG_ERROR, "Main server: MOTOR: Invalid MOTOR action specified.");
      SEND_MSG(sock, COMMAND_ERR_INV_ARG);
      return;
    }

    argc -= 3;
    argv += 3;
    switch (action) {
    case ACTION_MOTOR_SET:
    {
      if (argc < 1) {
        LOG_MSG(LOG_ERROR,
                "Main server: MOTOR: Not enough arguments provided (SET).");
        SEND_MSG(sock, COMMAND_ERR_INV_ARG);
        return;
      }

      char *err;
      double speed = strtod(argv[0], &err);
      if (errno == ERANGE || err == argv[0]) {
        LOG_MSG(LOG_ERROR, "Main server: MOTOR: Invalid motor speed.");
        SEND_MSG(sock, COMMAND_ERR_INV_ARG);
        return;
      }

      speed = CLAMP(speed, -1, 1);
      if (IS_OK(motor_controller_set_speed(
              &app.motor_controller,
              &app.system_settings.hardware.motors.motors[motor_id], speed))) {
        LOG_MSG(LOG_INFO, "Main server: MOTOR: Set motor speed successfully.");
        SEND_MSG(sock, COMMAND_OK);
        return;
      } else {
        LOG_MSG(LOG_ERROR, "Main server: MOTOR: Cannot set motor speed.");
        SEND_MSG(sock, COMMAND_ERR_UNK);
        return;
      }
    } break;
    case ACTION_MOTOR_STOP:
    {
      if (IS_OK(motor_controller_set_speed(
              &app.motor_controller,
              &app.system_settings.hardware.motors.motors[motor_id], 0))) {
        LOG_MSG(LOG_INFO, "Main server: MOTOR: Set motor speed successfully.");
        SEND_MSG(sock, COMMAND_OK);
        return;
      } else {
        LOG_MSG(LOG_ERROR, "Main server: MOTOR: Cannot set motor speed.");
        SEND_MSG(sock, COMMAND_ERR_UNK);
        return;
      }
    } break;
    case ACTION_MOTOR_DISABLE:
    {
      app.system_settings.hardware.motors.motors[motor_id].enabled = false;
    } break;
    case ACTION_MOTOR_ENABLE:
    {
      app.system_settings.hardware.motors.motors[motor_id].enabled = true;
    } break;
    }
  } else if (!strcmp(argv[0], "drive") || !strcmp(argv[0], "d")) {
    u8 args = 0;

    /* clang-format off */
    switch (app.system_settings.driving.mode) {
    case SINGLE:   args = 1; break; /* Forward/Back */
    case TANK:                      /* Left, Right */
    case QUADTANK:                  /* Left, Right */
    case MECANUM:  args = 4; break; /* Drive, Strafe, Twist */
    }
    /* clang-format on */

    if (argc < args + 1 || args == 0) {
      LOG_MSG(LOG_ERROR, "Main server: DRIVE: Not enough arguments provided.");
      SEND_MSG(sock, COMMAND_ERR_INV_ARG);
      return;
    }

    double *argv2 = calloc(1, sizeof(double) * args);

    char *err;
    i32 i;
    for (i = 0; i < args; i++) {
      double value = strtod(argv[i + 1], &err);
      if (errno == ERANGE || err == argv[i + 1]) {
        LOG_MSG(LOG_ERROR, "Main server: MOTOR: Invalid motor speed.");
        SEND_MSG(sock, COMMAND_ERR_INV_ARG);
        return;
      }
      argv2[i] = value;
    }

    /* FIXME: When loading configuration, check motor count for mode. */
    switch (app.system_settings.driving.mode) {
    case SINGLE:
      motor_controller_set_speed(&app.motor_controller,
                                 GET_APP_MOTOR(motor_top_left), argv2[0]);
      break;
    case TANK:
      motor_controller_set_speed(&app.motor_controller,
                                 GET_APP_MOTOR(motor_top_left), argv2[0]);
      motor_controller_set_speed(&app.motor_controller,
                                 GET_APP_MOTOR(motor_top_right), argv2[1]);
      break;
    case QUADTANK:
      motor_controller_set_speed(&app.motor_controller,
                                 GET_APP_MOTOR(motor_top_left), argv2[0]);
      motor_controller_set_speed(&app.motor_controller,
                                 GET_APP_MOTOR(motor_top_right), argv2[1]);
      motor_controller_set_speed(&app.motor_controller,
                                 GET_APP_MOTOR(motor_bottom_left), argv2[0]);
      motor_controller_set_speed(&app.motor_controller,
                                 GET_APP_MOTOR(motor_bottom_right), argv2[1]);
      break;
    case MECANUM:
      motor_controller_set_speed(&app.motor_controller,
                                 GET_APP_MOTOR(motor_top_left),
                                 argv2[0] + argv2[1] + argv2[2]);
      motor_controller_set_speed(&app.motor_controller,
                                 GET_APP_MOTOR(motor_top_right),
                                 argv2[0] - argv2[1] - argv2[2]);
      motor_controller_set_speed(&app.motor_controller,
                                 GET_APP_MOTOR(motor_bottom_left),
                                 argv2[0] - argv2[1] + argv2[2]);
      motor_controller_set_speed(&app.motor_controller,
                                 GET_APP_MOTOR(motor_bottom_right),
                                 argv2[0] + argv2[1] - argv2[2]);
      break;
    }

    free(argv2);
  } else {
    SEND_MSG(sock, COMMAND_ERR_UNK);
  }
}

void
parse_message(i32 i, char *msg, client_data_t *data, i32 sock)
{
  if (msg == nullptr) {
    LOG_MSG(LOG_ERROR, "parse_message: `msg` is NULL! Ignoring message.");
    return;
  }

  trim_string_fast(msg);
  if (!is_str_alphanumeric(msg)) {
    return;
  }

  char const *delim = " \t";

  i32 argc = 1;
  char *msg_clone[1024];
  strncpy((char *) msg_clone, msg, sizeof(msg_clone));
  char *argv[10] = { 0 }, *cur = strtok((char *) msg_clone, delim);
  argv[0] = cur;
  for (;;) {
    cur = strtok(nullptr, delim);
    if (cur == nullptr)
      break;

    if (*cur == '\0')
      break;

    argv[argc] = cur;
    if (argc >= 9) {
      LOG_MSG(LOG_WARNING, "Ignoring argument, reached argument limit.");
      break;
    }

    argc++;
  }

  switch (data->mode) {
  case MODE_AUTH: execute_auth(i, msg, sock, data, argc, argv); break;
  case MODE_COMMAND: execute_command(i, msg, sock, data, argc, argv); break;
  default:
    LOG_MSG(LOG_FATAL,
            "Cannot execute comand for user socket %d: Invalid mode!", sock);
  }
}

void *
main_server_loop(void *data)
{
  (void) data;

  client_data_t c_data[FD_SETSIZE] = {
    {
     .mode = MODE_AUTH,
     }
  };

  i32 opt = true;
  i32 master_socket, addrlen, new_socket, client_socket[30],
      max_clients = 30, activity, i, valread, sd;
  i32 max_sd;
  struct sockaddr_in address;

  char buffer[1025] = { 0 };
  fd_set readfds;

  const char message[20];
  snprintf((char *) message, 20, "YROSauth%s\r\n", app.version);

  for (i = 0; i < max_clients; i++) client_socket[i] = 0;

  if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    LOG_MSG(LOG_FATAL, "Main server: socket error: %s", strerror(errno))
    return nullptr;
  }

  if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &opt,
                 sizeof(opt)) < 0) {
    LOG_MSG(LOG_FATAL, "Main server: setsockopt error: %s", strerror(errno))
    return nullptr;
  }

  address.sin_family      = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port        = htons(app.running_port);

  if (bind(master_socket, (struct sockaddr *) &address, sizeof(address)) < 0) {
    LOG_MSG(LOG_FATAL, "Main server: Bind error: %s", strerror(errno))
    return nullptr;
  }

  if (listen(master_socket, 3) < 0) {
    LOG_MSG(LOG_FATAL, "Main server: Listen error: %s", strerror(errno))
    return nullptr;
  }

  addrlen = sizeof(address);

  for (;;) {
    FD_ZERO(&readfds);

    FD_SET(master_socket, &readfds);
    max_sd = master_socket;

    for (i = 0; i < max_clients; i++) {
      sd = client_socket[i];

      if (sd > 0)
        FD_SET(sd, &readfds);

      if (sd > max_sd)
        max_sd = sd;
    }

    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if ((activity < 0) && (errno != EINTR))
      LOG_MSG(LOG_FATAL, "Main server: Select error: %s", strerror(errno));

    if (FD_ISSET(master_socket, &readfds)) {
      if ((new_socket = accept(master_socket, (struct sockaddr *) &address,
                               (socklen_t *) &addrlen)) < 0) {
        LOG_MSG(LOG_FATAL, "Main server: Accept error: %s", strerror(errno));
        return nullptr;
      }

      LOG_MSG(LOG_INFO, "Main server: New connection: %s:%d",
              inet_ntoa(address.sin_addr), ntohs(address.sin_port));

      c_data[i].mode = MODE_AUTH;

      if (send(new_socket, message, strlen(message), 0) != strlen(message)) {
        LOG_MSG(LOG_ERROR, "Main server: Failed sending welcome message: %s",
                strerror(errno));
        return nullptr;
      }

      for (i = 0; i < max_clients; i++) {
        if (client_socket[i] == 0) {
          client_socket[i] = new_socket;
          break;
        }
      }
    }

    for (i = 0; i < max_clients; i++) {
      sd = client_socket[i];

      if (FD_ISSET(sd, &readfds)) {
        if ((valread = read(sd, buffer, 1024)) == 0) {
          c_data[i].mode = MODE_AUTH;
          getpeername(sd, (struct sockaddr *) &address, (socklen_t *) &addrlen);
          LOG_MSG(LOG_INFO, "Main server: Client disconnected: %s:%d",
                  inet_ntoa(address.sin_addr), ntohs(address.sin_port));

          close(sd);
          client_socket[i] = 0;
        } else {
          buffer[valread] = '\0';
          parse_message(i, buffer, &c_data[i], sd);
          /* FIXME: Remove this if unused.
          if (response.action == ResponseAction::Close)
            goto sock_disconnect;
          */
        }
      }
    }
  }

  return 0;
}

pthread_t *
start_main_server(void)
{
  LOG_MSG(LOG_DEBUG, "Starting main server port %d.", app.running_port);
  pthread_create(&thread_main, nullptr, &main_server_loop, nullptr);
  return &thread_main;
}
