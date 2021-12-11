#pragma once

#include <Servers/Server.h>
#include <netinet/in.h>

namespace Server {

class UDP : public Server {
    // TODO: Implement
public:
    void run();
private:
    int m_socket_fd;
    sockaddr_in m_serv;
};

}
