#include <Signals.h>

#include <cstdlib>
#include <thread>

#include <Common.h>
#include <Config.h>
#include <Application.h>

using namespace std::chrono_literals;

void reload_signal_handler(int signum)
{
    (void)signum;
    spdlog::info("Reloading configuration file...");
    Configuration::the().reload();
}

void clean_exit(int signum)
{
    spdlog::info("Got kill signal, exiting...");
    Application::App::the().stop();
    std::this_thread::sleep_for(1s);
    exit(EXIT_FAILURE);
}

void initialize_signals()
{
    signal(1, reload_signal_handler);
    signal(SIGTERM, clean_exit);
    signal(SIGINT, clean_exit);
}
