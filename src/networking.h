#ifndef NETWORKING_H
#define NETWORKING_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "common.h"

#include <NetworkManager.h>
#include <stdbool.h>

typedef struct wifi_network {
  char *ssid;
  char *password;
} wifi_network_t;

typedef struct wifi_network_list {
  int cnt;
  wifi_network_t *networks;
} wifi_network_list_t;

typedef struct ipv4_addr {
  u8 a, b, c, d;
} ipv4_addr_t;

bool add_wifi_network_if_not_exist(NMClient *client, wifi_network_t network);
bool rescan_wifi_networks(NMClient *client);

pthread_t *start_broadcasting(void);
void stop_broadcasting(void);

#if defined(__cplusplus)
}
#endif

#endif // NETWORKING_H
