#pragma once

#include <Common.h>
#include <Servers/Server.h>

#include <Drivers/MotorDriver.h>
#include <Drivers/PWMServo.h>

namespace Server {

extern Driver::MotorDriver* motor_left;
extern Driver::MotorDriver* motor_right;
extern Driver::PWMServo* servo_controller;

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
