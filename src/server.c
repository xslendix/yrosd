#include "server.h"
#include "src/logging.h"

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

void
execute_auth(int i, char *msg, int sock, client_data_t *data, int argc, char *argv[])
{ }

void
execute_setup(int i, char *msg, int sock, client_data_t *data, int argc, char *argv[])
{ }

void
execute_command(int i, char *msg, int sock, client_data_t *data, int argc, char *argv[])
{ }

void
parse_message(int i, char *msg, client_data_t *data, int sock)
{
  char const *delim = " \t";

  int argc = 0;
  char *argv[10], *cur = strtok(msg, delim);
  for (;;) {
    cur = strtok(nullptr, delim);
    if (*cur == '\0')
      continue;

    if (cur == nullptr)
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
  fd_set fds, readfds;
  int i, client_addr_len;
  int client_sock[2], rc, num_socks = 0, max_socks = 2;

  int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server_sock == -1)
    LOG_MSG(LOG_FATAL, "Socket");

  struct sockaddr_in server_addr, client_addr;  
  memset(&server_addr, 0, sizeof(struct sockaddr_in));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(6782);

  if (-1 == bind(server_sock, (struct sockaddr *)&server_addr, 
                 sizeof(struct sockaddr_in))) 
    LOG_MSG(LOG_FATAL, "Bind");

  if (-1 == listen(server_sock, SOMAXCONN))
    LOG_MSG(LOG_FATAL, "Listen");

  FD_ZERO(&fds);
  FD_SET(server_sock, &fds);

  while(1) {
    readfds = fds;
    rc = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);

    if (rc == -1) {
      LOG_MSG(LOG_FATAL, "Select");
      break;
    }

    for (i = 0; i < FD_SETSIZE; i++) {
      if (FD_ISSET(i, &readfds)) {
        if (i == server_sock) {
          if (num_socks < max_socks) {
            client_sock[num_socks] = accept(server_sock,
                                          (struct sockaddr *) &client_addr,
                                          (socklen_t *) &client_addr_len);
            if (client_sock[num_socks] == -1)
              LOG_MSG(LOG_FATAL, "Accept");
            FD_SET(client_sock[num_socks], &fds);
            num_socks++;
          } else {
            LOG_MSG(LOG_DEBUG, "Ran out of socket space.");
          }
        } else {
          int const message_len = 1024;
          char message[message_len+1];
          int in, index = 0, limit = message_len+1;

          while ((in = recv(client_sock[i], &message[index], limit, 0)) > 0) {
            index += in;
            limit -= in;
          }

          parse_message(i, message, &c_data[i], client_sock[i]);
          LOG_MSG(LOG_DEBUG, "%d - `%s`", index, message);
        }
      }
    }
  }

  close(server_sock);
  return 0;
}

pthread_t *
start_main_server(void)
{
  pthread_create(&thread_main, nullptr, &main_server_loop, nullptr);
  return &thread_main;
}
