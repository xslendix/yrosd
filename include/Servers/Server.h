#pragma once

namespace Server {

class Server {
public:
    virtual void Run();

    virtual void Stop();

protected:
    bool m_running = false;
};

}
