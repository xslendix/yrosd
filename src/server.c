#include "server.h"

#include "src/common.h"
#include "src/logging.h"
#include "src/yrosd.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>

#include <pthread.h>

pthread_t thread_main;

typedef struct client_data {
  c_mode_t mode;
} client_data_t;

// FIXME: CRITICAL: Put this in the user configuration or something lol. Also, should be hashed.
char *const PASSWORD = "password";

#define SEND_MSG(sock, msg) BREAKABLE_SCOPE(){ \
    char const *_msg = msg; \
    if ((ret = send(sock, _msg, strlen(_msg), 0)) != strlen(_msg)) { \
      LOG_MSG(LOG_ERROR, "Main server: Failed sending message: %s", strerror(errno)); \
      break; \
    } \
}

char const *SETUP_OK              = (char[]){ 0x00, '\r', '\n', '\0' }; // Data on the server has been processed correctly.
char const *SETUP_CLIENT_OK       = (char[]){ 0x01, '\r', '\n', '\0' }; // Data on the client has been processed correctly.
char const *SETUP_ERR_INVALID     = (char[]){ 0x02, '\r', '\n', '\0' }; // Invalid configuration provided.
char const *AUTH_PASS_FAIL        = (char[]){ 0x10, '\r', '\n', '\0' }; // Authentication failed. Please try again.
char const *AUTH_PASS_OK          = (char[]){ 0x10, '\r', '\n', '\0' }; // Authentication succeeded.
char const *COMMAND_OK            = (char[]){ 0x20, '\r', '\n', '\0' }; // Command executed successfully.
char const *COMMAND_ERR_UNK       = (char[]){ 0x21, '\r', '\n', '\0' }; // An unknown error occured.
char const *COMMAND_ERR_ARGC      = (char[]){ 0x22, '\r', '\n', '\0' }; // Not enough arguments.
char const *COMMAND_ERR_INV_ARG   = (char[]){ 0x23, '\r', '\n', '\0' }; // Invalid argument(s).
char const *COMMAND_ERR_NOT_FOUND = (char[]){ 0x24, '\r', '\n', '\0' }; // Cannot find data/target specified.
char const *COMMAND_ERR_CFG       = (char[]){ 0x35, '\r', '\n', '\0' }; // Command not configured.

void
execute_auth(int i, char *msg, int sock, client_data_t *data, int argc, char *argv[])
{
  int ret;
  if (strcmp(msg, PASSWORD) == 0) {
    LOG_MSG(LOG_INFO, "New client logged in successfully!");
    SEND_MSG(sock, AUTH_PASS_OK);
    data->mode = MODE_COMMAND;
  } else {
    LOG_MSG(LOG_INFO, "New client failed to log in! Either a bad configuration on client side or a possible attack.");
    SEND_MSG(sock, AUTH_PASS_FAIL);
  }
}

void
execute_setup(int i, char *msg, int sock, client_data_t *data, int argc, char *argv[])
{ }

void
execute_command(int i, char *msg, int sock, client_data_t *data, int argc, char *argv[])
{
  int ret;

  if (argc < 1) {
    send(sock, COMMAND_ERR_ARGC, strlen(COMMAND_ERR_ARGC), 0);
    return;
  }

  to_lower_string(argv[0]);
  LOG_MSG(LOG_DEBUG, "Command: `%s` by socket %d", argv[0], sock);
  if (!strcmp(argv[0], "info")) {
    char features[50] = "Features=[";
    if (app.system_settings.video_streaming.enabled)
      snprintf(features+10, sizeof(features)-10, "'VideoStreaming']");
    else {
      features[10] = ']';
      features[11] = '\0';
    }
    SEND_MSG(sock, text_format("%s\r\n"
                               "RobotName='%s'\r\n"
                               "DrivingMode='%s'\r\n%s",
                       features, app.user_settings.general.name, drive_mode_str(app.system_settings.driving.mode), COMMAND_OK));
  } else {
    SEND_MSG(sock, COMMAND_ERR_UNK);
  }
}

void
parse_message(int i, char *msg, client_data_t *data, int sock)
{
  if (msg == nullptr) {
    LOG_MSG(LOG_ERROR, "parse_message: `msg` is NULL! Ignoring message.");
    return;
  }
  trim_string_fast(msg);

  char const *delim = " \t";

  int argc = 1;
  char *msg_clone[1024];
  strncpy((char *)msg_clone, msg, sizeof(msg_clone));
  char *argv[10] = { 0 }, *cur = strtok((char* )msg_clone, delim);
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
    case MODE_AUTH:    execute_auth(i, msg, sock, data, argc, argv);    break;
    case MODE_SETUP:   execute_setup(i, msg, sock, data, argc, argv);   break;
    case MODE_COMMAND: execute_command(i, msg, sock, data, argc, argv); break;
    default:           LOG_MSG(LOG_FATAL, "Cannot execute comand for user socket %d: Invalid mode!", sock);
  }
}

void *
main_server_loop(void *data)
{
  (void) data;

  client_data_t c_data[FD_SETSIZE] = {{
    .mode = MODE_AUTH,
  }};

  int opt = true;
  int master_socket, addrlen, new_socket, client_socket[30],
      max_clients = 30, activity, i, valread, sd;
  int max_sd;
  struct sockaddr_in address;

  char buffer[1025] = { 0 };
  fd_set readfds;

  const char message[20];
  snprintf((char *)message, 20, "YROSauth%s\r\n", app.version);

  for (i = 0; i < max_clients; i++)
    client_socket[i] = 0;

  if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    LOG_MSG(LOG_FATAL, "Main server: socket error: %s", strerror(errno))
    return nullptr;
  }

  if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
    LOG_MSG(LOG_FATAL, "Main server: setsockopt error: %s", strerror(errno))
    return nullptr;
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(app.running_port);

  if (bind(master_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
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
      if ((new_socket = accept(master_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        LOG_MSG(LOG_FATAL, "Main server: Accept error: %s", strerror(errno));
        return nullptr;
      }

      LOG_MSG(LOG_INFO, "Main server: New connection: %s:%d", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

      if (send(new_socket, message, strlen(message), 0) != strlen(message)) {
        LOG_MSG(LOG_ERROR, "Main server: Failed sending welcome message: %s", strerror(errno));
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
        sock_disconnect:
          getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
          LOG_MSG(LOG_INFO, "Main server: Host disconnected: %s:%d", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

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
