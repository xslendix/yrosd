#include <Application.h>
#include <Backend/Backend.h>
#include <Common.h>
#include <Drivers/MotorDriver.h>
#include <spdlog/spdlog.h>

namespace Driver {

MotorDriver::MotorDriver(uint pwm_pin, uint flt_pin, uint enable_pin, uint dir_pin)
    : m_pwm(pwm_pin)
    , m_flt(flt_pin)
    , m_en(enable_pin)
    , m_dir(dir_pin)
{
  App::the().backend()->SetPullUpDown(m_flt, 2); // PUD_UP
  SetEnable(true);
}

void
MotorDriver::SetSpeed(int speed)
{
  spdlog::debug("MotorDriver::SetSpeed({})", speed);

  const int max_speed = 480;

  bool backward = 0;

  if (speed < 0) {
    speed *= -1;
    backward = 1;
  }

  speed = min(speed, max_speed);

  spdlog::debug("    backward: {}, speed: {}", backward, speed);

  App::the().backend()->SetGPIO(m_dir, backward);
  App::the().backend()->SetHardwarePWM(m_pwm, 20000, speed * 6250 / 3);
}

void
MotorDriver::SetEnable(bool enable)
{
  App::the().backend()->SetGPIO(m_en, enable);
}

bool
MotorDriver::GetFault()
{
  return !App::the().backend()->GetGPIO(m_flt);
}

MotorDriver::~MotorDriver()
{
}

}
