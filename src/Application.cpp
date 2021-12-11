#include <thread>

#include <Common.h>
#include <Application.h>
#include <Servers/Servers.h>

namespace Application {

static unique_ptr<App> s_the = nullptr;

App& App::the()
{
    if (!s_the) {
        unique_ptr<App> new_the(std::make_unique<App>());
        s_the = std::move(new_the);
    }
    return *s_the;
}

void App::run()
{
    spdlog::debug("Reached App::run()");

    std::thread main_server_thread(&Server::Main::run, m_main_server);
    std::thread udp_server_thread(&Server::UDP::run, m_udp_server);
    std::thread video_server_thread(&Server::Video::run, m_video_server);

    main_server_thread.join();
    udp_server_thread.join();
    video_server_thread.join();
}

void App::stop()
{
    m_main_server.stop();
    m_udp_server.stop();
    m_video_server.stop();
}

}
