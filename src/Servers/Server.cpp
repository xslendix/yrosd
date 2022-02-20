#include <Servers/Server.h>

namespace Server {

void Server::Run()
{
    m_running = true;
};

void Server::Stop()
{
    m_running = false;
};

}
