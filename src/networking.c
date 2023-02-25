#include "networking.h"

#include "common.h"
#include "logging.h"
#include "yrosd.h"

#include <NetworkManager.h>
#include <arpa/inet.h>
#include <glib.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

pthread_t thread_broadcasting;

char *hotspot_uuid = "2b0d0f1d-b79d-43af-bde1-71744625642e";
NMConnection *current_hotspot_connection = nullptr;

extern struct ether_addr *ether_aton(char const *);

static void
broadcast(char const *mess, u16 port)
{
  struct sockaddr_in s;

  i32 broadcast_socket = socket(AF_INET, SOCK_DGRAM, 0);
  i32 so_broadcast     = 1;
  i32 so_reuseaddr     = 1;

  if (broadcast_socket < 0)
    LOG_MSG(LOG_FATAL, "Cannot open broadcast socket! %s", strerror(errno));

  setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, &so_broadcast,
             sizeof(so_broadcast));
  setsockopt(broadcast_socket, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr,
             sizeof(so_reuseaddr));

  memset(&s, 0, sizeof(s));
  s.sin_family      = AF_INET;
  s.sin_port        = htons(port);
  s.sin_addr.s_addr = htonl(INADDR_BROADCAST);

  LOG_MSG(LOG_DEBUG, "Sending broadcast message port %hd", port)
  if (sendto(broadcast_socket, mess, strlen(mess), 0, (struct sockaddr *) &s,
             sizeof(s)) < 0)
    LOG_MSG(LOG_FATAL, "sendto: %s", strerror(errno));
}

char *bmessage = nullptr;

void *
broadcast_loop(void *data)
{
  (void) data;

  for (;;) {
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr);
    broadcast(bmessage, 9901);
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

  snprintf(bmessage, 50, "YROSDS%s\n%hd", app.version, app.running_port);

  pthread_create(&thread_broadcasting, nullptr, &broadcast_loop, nullptr);
  return &thread_broadcasting;
}

void
stop_broadcasting(void)
{
  if (bmessage)
    free(bmessage);
  bmessage = nullptr;

  pthread_cancel(thread_broadcasting);
}

static void
activation_callback(GObject *source_object, GAsyncResult *res,
                    gpointer user_data)
{
  NMActiveConnection *conn;
  GError *error   = NULL;
  GMainLoop *loop = user_data;

  conn = nm_client_activate_connection_finish(NM_CLIENT(source_object), res,
                                              &error);

  if (!conn) {
    g_warning("Failed to activate connection: (%d) %s", error->code,
              error->message);
    g_error_free(error);
    return;
  } else {
    LOG_MSG(LOG_INFO, "Activated connection.");
    fflush(stdout);
  }

  g_main_loop_quit(loop);
}

NMConnection *
validate_current_hotspot_connection(NMClient *client)
{
  LOG_MSG(LOG_INFO, "Validating hotspot.");

  i32 i;
  GPtrArray const *connections = nm_client_get_connections(client);
  current_hotspot_connection   = nullptr;
  for (i = 0; i < connections->len; i++) {
    NMConnection *connection = connections->pdata[i];
    NMSettingConnection *settings =
        nm_connection_get_setting_connection(connection);
    char const *uuid = nm_setting_connection_get_uuid(settings);
    printf("UUID: %s HOTSPOT: %s\n", uuid, hotspot_uuid);
    if (!strcmp(uuid, hotspot_uuid)) {
      puts("FOUND!");
      current_hotspot_connection = connection;
      break;
    }
  }

  if (current_hotspot_connection) {
    GMainLoop *loop  = g_main_loop_new(NULL, FALSE);
    NMDevice *device = nm_client_get_device_by_iface(client, "wlan0");
    if (!device)
      LOG_MSG(LOG_FATAL, "Cannot find WLAN device!");
    nm_client_activate_connection_async(client, current_hotspot_connection,
                                        device, NULL, NULL, activation_callback,
                                        loop);
    g_main_loop_run(loop);
  }

  return current_hotspot_connection;
}

gboolean connected = FALSE;

static void
on_connection_added(GObject *source_object, GAsyncResult *res,
                    gpointer user_data)
{
  NMActiveConnection *conn;
  GError *error   = NULL;
  GMainLoop *loop = user_data;

  conn = nm_client_add_and_activate_connection_finish(NM_CLIENT(source_object),
                                                      res, &error);

  if (!conn) {
    g_warning("Failed to add new connection: (%d) %s", error->code,
              error->message);
    g_error_free(error);
    return;
  } else {
    LOG_MSG(LOG_INFO, "Added new connection.");
    fflush(stdout);
  }

  g_main_loop_quit(loop);
}

NMConnection *
create_and_activate_hotspot(NMClient *client, const char *ssid)
{
  validate_current_hotspot_connection(client);

  if (ssid == nullptr)
    ssid = "YROS Robot";

  LOG_MSG(LOG_INFO, "Starting wireless hotspot. SSID: %s", ssid);

  /* We already have a hotspot active, just return that connection. */
  if (current_hotspot_connection)
    return current_hotspot_connection;

  GMainLoop *loop = g_main_loop_new(NULL, FALSE);

  NMDevice *wlan_device = nm_client_get_device_by_iface(client, "wlan0");
  if (!wlan_device)
    LOG_MSG(LOG_FATAL, "Cannot find WLAN device!");

  NMConnection *connection = nm_simple_connection_new();
  NMSettingConnection *s_con =
      (NMSettingConnection *) nm_setting_connection_new();
  g_object_set(s_con, "type", "802-11-wireless", "uuid", hotspot_uuid, "id",
               "RoboHotspot", "autoconnect", FALSE, NULL);
  nm_connection_add_setting(connection, (NMSetting *) s_con);

  NMSettingWireless *s_wifi = (NMSettingWireless *) nm_setting_wireless_new();
  g_object_set(s_wifi, "mode", "ap", NULL);

  char const *str_mac =
      nm_device_wifi_get_permanent_hw_address(NM_DEVICE_WIFI(wlan_device));
  struct ether_addr *bin_mac = ether_aton(str_mac);
  if (bin_mac) {
    GByteArray *hw_address;
    hw_address = g_byte_array_sized_new(ETH_ALEN);
    g_byte_array_append(hw_address, bin_mac->ether_addr_octet, ETH_ALEN);
    g_object_set(s_wifi, "mac-address", hw_address, NULL);
    g_byte_array_unref(hw_address);
  }
  nm_connection_add_setting(connection, (NMSetting *) s_wifi);

  NMSettingIP4Config *s_ipv4 =
      (NMSettingIP4Config *) nm_setting_ip4_config_new();
  g_object_set(s_ipv4, "method", "shared", NULL);
  nm_connection_add_setting(connection, (NMSetting *) s_ipv4);

  NMSettingIP6Config *s_ipv6 =
      (NMSettingIP6Config *) nm_setting_ip6_config_new();
  g_object_set(s_ipv6, "method", "ignore", NULL);
  nm_connection_add_setting(connection, (NMSetting *) s_ipv6);

  GBytes *ssid_b = g_bytes_new(ssid, strlen(ssid) + 1);
  g_object_set(s_wifi, "ssid", ssid_b, NULL);
  g_bytes_unref(ssid_b);

  nm_client_add_and_activate_connection_async(
      client, connection, wlan_device, NULL, NULL, on_connection_added, loop);

  g_main_loop_run(loop);

  return current_hotspot_connection;
}
