#include <Application.h>
#include <Common.h>
#include <Servers/Servers.h>
#include <Backend/Backend.h>

#include <thread>

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

void App::Run()
{
    spdlog::debug("Reached App::Run()");

    if (!m_backend) {
        unique_ptr<Backend::BaseBackend> new_backend(std::make_unique<Backend::Backend>());
        m_backend = std::move(new_backend);
    }

    std::thread main_server_thread(&Server::Main::Run, m_main_server);

    std::thread video_server_thread(&Server::Video::Run, m_video_server);

    spdlog::info("Starting video...");
    video_server_thread.join();
    spdlog::info("Starting main...");
    main_server_thread.join();
}

void App::Stop()
{
    m_main_server.Stop();
    m_video_server.Stop();
}

}
