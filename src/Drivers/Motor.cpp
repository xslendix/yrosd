#include "Application.h"
#include <Common.h>
#include <Drivers/Motor.h>
#include <Backend/Backend.h>

namespace Driver {

Motor::Motor(uint pwm_pin, uint dir_pin, uint enable_pin, uint flt_pin)\
    : m_pwm(pwm_pin), m_flt(flt_pin), m_en(enable_pin), m_dir(dir_pin)
{
    App::the().backend()->SetPullUpDown(m_flt, 2); // PUD_UP
    SetEnable(true);
}

void Motor::SetSpeed(int speed)
{
    const int max_speed = 480;

    bool forward = false;

    if (speed < 0) {
        speed *= -1;
        forward = 1;
    }

    speed = speed > max_speed ? max_speed : speed;

    App::the().backend()->SetGPIO(m_dir, forward);
    App::the().backend()->SetHardwarePWM(m_pwm, 20000, speed * 6250 / 3);
}

void Motor::SetEnable(bool enable)
{
    App::the().backend()->SetGPIO(m_en, enable);
}

bool Motor::GetFault()
{
    return !App::the().backend()->GetGPIO(m_flt);
}

Motor::~Motor()
{ }

}
