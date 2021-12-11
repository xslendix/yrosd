#include <Servers/UDPServer.h>

#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <Common.h>
#include <CommandParser.h>
#include <Config.h>
#include <String.h>

namespace Server {

void UDP::run()
{
    Server::run();

    if ((m_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        spdlog::error("UDPServer: Failed to create movement socket: {}", strerror(errno));
        m_running = false;
        return;
    }

    memset(&m_serv, '\0', sizeof(m_serv));
    m_serv.sin_family = AF_INET;
    m_serv.sin_port = htons(Configuration::the().server()->port_udp);
    m_serv.sin_addr.s_addr = inet_addr(Configuration::the().server()->host.c_str());

    int enabled = 1;
    if (setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(int)) < 0) {
        spdlog::error("UDP: Failed executing setsockopt: {}", strerror(errno));
        return;
    }

    if (bind(m_socket_fd, (sockaddr*)&m_serv, sizeof(m_serv)) < 0) {
        spdlog::error("MovementServer: Failed to bind socket: {}", strerror(errno));
        m_running = false;
        return;
    }

    m_running = true;

    sockaddr_in client;
    memset(&client, '\0', sizeof(client));

    spdlog::info("UDP server started. Awaiting commands.");

    auto buffer = new char[255];
    while (m_running) {
        socklen_t l = sizeof(client);

        int rc = recvfrom(m_socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &l);

        if (rc < 0) {
            spdlog::error("MovementServer: Failed reading from socket.");
        }
        string buf(buffer);
        trim(buf);

    }

    close(m_socket_fd);
}

}
