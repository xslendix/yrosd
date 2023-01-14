#include "server.h"

#include <unistd.h>

#include <pthread.h>

pthread_t thread;

void *
main_server_loop(void *data)
{
  (void) data;

  for (;;) {
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr);
    sleep(1);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
    sleep(1);
  }
}

pthread_t *
start_main_server(void)
{
  pthread_create(&thread, nullptr, &main_server_loop, nullptr);
  return &thread;
}
