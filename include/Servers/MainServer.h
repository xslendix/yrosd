#pragma once

#include <Common.h>
#include <Servers/Server.h>

namespace Server {

class Main : public Server {
public:
    void Run();
    void Stop();

private:
    enum class ResponseAction {
        None,
        Close,
    };

    struct Response {
        ResponseAction action;
        string response;
    };

    Response ParseInstruction(string instruction);
};

}
