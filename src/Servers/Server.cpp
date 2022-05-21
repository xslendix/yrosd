#include <Servers/Server.h>

#include <spdlog/spdlog.h>

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
