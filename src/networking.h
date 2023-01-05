#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h>

#include <NetworkManager.h>

typedef struct wifi_network {
  char *ssid;
  char *password;
} wifi_network_t;

typedef struct wifi_network_list {
  int cnt;
  wifi_network_t *networks;
} wifi_network_list_t;

bool AddWiFiNetworkIfNotExist(NMClient *client, wifi_network_t network);
bool RescanWiFiNetworks(NMClient *client);

#if defined(__cplusplus)
}
#endif

