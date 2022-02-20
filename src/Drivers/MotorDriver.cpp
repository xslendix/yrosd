#include <Application.h>
#include <Backend/Backend.h>
#include <Common.h>
#include <Drivers/MotorDriver.h>

namespace Driver {

MotorDriver::MotorDriver(uint pwm_pin, uint dir_pin, uint enable_pin, uint flt_pin)
    : m_pwm(pwm_pin)
    , m_flt(flt_pin)
    , m_en(enable_pin)
    , m_dir(dir_pin)
{
    App::the().backend()->SetPullUpDown(m_flt, 2); // PUD_UP
    SetEnable(true);
}

void MotorDriver::SetSpeed(int speed)
{
    std::cout<<"MotorDriver::SetSpeed(" << speed << "): backward=";

    const int max_speed = 480;

    bool backward = 0;

    if (speed < 0) {
        speed *= -1;
        backward = 1;
    }

    speed = speed > max_speed ? max_speed : speed;

    std::cout << backward << ", speed=" << speed << std::endl;

    App::the().backend()->SetGPIO(m_dir, backward);
    App::the().backend()->SetHardwarePWM(m_pwm, 20000, speed * 6250 / 3);
}

void MotorDriver::SetEnable(bool enable)
{
    App::the().backend()->SetGPIO(m_en, enable);
}

bool MotorDriver::GetFault()
{
    return !App::the().backend()->GetGPIO(m_flt);
}

MotorDriver::~MotorDriver()
{
}

}
