#pragma once

#include <Backend/BaseBackend.h>
#include <Common.h>
#include <Config.h>
#include <Servers/Servers.h>

#include <thread>

namespace Application {

class App {
public:
    void Run();
    void Stop();

    static App& the();

    Backend::BaseBackend* backend() const { return m_backend.get(); }

private:
    Server::Main m_main_server;
    Server::Video m_video_server;

    unique_ptr<Backend::BaseBackend> m_backend;
};

}

using Application::App;
