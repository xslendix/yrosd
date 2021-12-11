#pragma once

namespace Server {

class Server {
public:
    virtual void run();

    virtual void stop();

protected:
    bool m_running = false;
};

}
