#pragma once

#include <Backend/Backend.h>
#include <Common.h>
#include <Config.h>
#include <Servers/Servers.h>

#include <thread>

namespace Application {

class App {
public:
    void run();
    void stop();

    static App& the();

private:
    Server::Main m_main_server;
    Server::Video m_video_server;
};

}

using Application::App;
