#include "setup.h"

#include "logging.h"
#include "settings.h"
#include "yrosd.h"

#include <sys/socket.h>
#include <unistd.h>

user_settings_t
do_setup_mode(char *data_out)
{
  data_out = nullptr;

  user_settings_t settings = { 0 };
restart:
  settings.is_valid = false;

  LOG_MSG(LOG_INFO, "Setup: Starting server.");

  /* 1. Create TCP socket. */
  i32 sock        = socket(AF_INET, SOCK_STREAM, 0);
  i32 const reuse = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse,
                 sizeof(reuse)) < 0) {
    LOG_MSG(LOG_FATAL, "Setup: setsockopt error: %s", strerror(errno));
    close(sock);
    goto restart;
  }

  struct sockaddr_in server_addr = { 0 }, client_addr = { 0 };
  server_addr.sin_family      = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port        = htons(9901);

  if (bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
    close(sock);
    LOG_MSG(LOG_FATAL, "Setup: Failed binding socket: %s", strerror(errno));
    goto restart;
  }

  if (listen(sock, 1) != 0) {
    close(sock);
    LOG_MSG(LOG_FATAL, "Setup: Failed listening: %s", strerror(errno));
    goto restart;
  }

  socklen_t opt = sizeof(client_addr);
  LOG_MSG(LOG_INFO, "Setup: Awaiting connection.");
  int client = accept(sock, (struct sockaddr *) &client_addr, &opt);
  if (client < 0) {
    close(sock);
    LOG_MSG(LOG_FATAL, "Setup: Failed accepting socket connection: %s",
            strerror(errno));
    goto restart;
  }

  /* 2. Send magic header. */
  const char message[25];
  snprintf((char *) message, 25, "YROSsetup%s\r\n", app.version);
  if (send(client, message, strlen(message), 0) < 0) {
    LOG_MSG(LOG_ERROR, "Setup: Failed sending welcome message: %s",
            strerror(errno));
    close(client);
    close(sock);
    goto restart;
  }

  /* 3. Get configuration file and validate. */
  char *buf         = calloc(1, 1024);
  size_t bytes_read = read(client, buf, sizeof(buf));

  if (bytes_read <= 0) {
    LOG_MSG(LOG_ERROR, "Setup: No data received: %s", strerror(errno));
    close(client);
    close(sock);
    goto restart;
  }

  settings = load_user_settings_string(buf);

  if (!settings.is_valid) {
    close(client);
    close(sock);
    goto restart;
  }

  close(client);
  close(sock);

  data_out = buf;

  return settings;
}
