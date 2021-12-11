#include <Servers/Server.h>

namespace Server {

void Server::run() {
    m_running = true;
};

void Server::stop() {
    m_running = false;
};

}
