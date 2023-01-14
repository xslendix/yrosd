#include "networking.h"

#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "src/logging.h"
#include "yrosd.h"
#include "common.h"

pthread_t thread;

static void
broadcast(char const *mess, u16 port)
{
    struct sockaddr_in s;

    int broadcast_socket = socket(AF_INET, SOCK_DGRAM, 0);
    int so_broadcast = 1;
    int so_reuseaddr = 1;

    if(broadcast_socket < 0)
      LOG_MSG(LOG_FATAL, "Cannot open broadcast socket! %s", strerror(errno));

    setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast));
    setsockopt(broadcast_socket, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr));

    memset(&s, 0, sizeof(s));
    s.sin_family = AF_INET;
    s.sin_port = htons(port);
    s.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    if(bind(broadcast_socket, (struct sockaddr*)&s, sizeof(s)) < 0)
      LOG_MSG(LOG_FATAL, "Cannot bind socket! %s", strerror(errno));

    LOG_MSG(LOG_DEBUG, "Sending broadcast message port %hd", port)
    if (sendto(broadcast_socket, mess, strlen(mess), 0, (struct sockaddr *)&s, sizeof(s)) < 0)
      LOG_MSG(LOG_FATAL, "sendto: %s", strerror(errno));
}


char *bmessage = nullptr;

void *
broadcast_loop(void *data)
{
  (void) data;

  for (;;) {
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr);
    broadcast(bmessage, app.running_port);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
    sleep(5);
  }
}

pthread_t *
start_broadcasting(void)
{
  bmessage = malloc(sizeof(char) * 50);
  if (!bmessage)
    LOG_MSG(LOG_FATAL, "Cannot allocate memory for broadcast message!");

  snprintf(bmessage, 50, "YROSDS%s\n%hhd.%hhd.%hhd.%hhd:%hd", app.version, app.running_ip.a, app.running_ip.b, app.running_ip.c, app.running_ip.d, app.running_port);

  pthread_create(&thread, nullptr, &broadcast_loop, nullptr);
  return &thread;
}

void
stop_broadcasting(void)
{
  if (bmessage)
    free(bmessage);
  bmessage = nullptr;

  pthread_cancel(thread);
}
