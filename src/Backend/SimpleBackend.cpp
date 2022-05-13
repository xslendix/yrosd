#if PREFERRED_BACKEND == 1

#include <Backend/SimpleBackend.h>

#include <Common.h>

namespace Backend {

void SimpleBackend::init()
{
    spdlog::debug("SimpleBackend: Initialized.");
}

void SimpleBackend::SetGPIOMode(u8 gpio, GpioMode mode)
{
    spdlog::debug("SimpleBackend: GPIO mode requested: {} for {}.", (uint)mode, gpio);
}

bool SimpleBackend::GetGPIO(u8 gpio)
{
    spdlog::debug("SimpleBackend: GPIO value requested for {}.", gpio);
    return 0;
}

void SimpleBackend::SetGPIO(u8 gpio, bool state)
{
    spdlog::debug("SimpleBackend: GPIO value change requested: {} for {}", state, gpio);
}

void SimpleBackend::SetPullUpDown(u8 gpio, uint pud)
{
    spdlog::debug("SimpleBackend: GPIO pull up down change requested: {} for {}", pud, gpio);
}

void SimpleBackend::SetHardwarePWM(u8 gpio, uint pwm, uint pwm_duty)
{
    spdlog::debug("SimpleBackend: GPIO hardware PWM change requested: {}, {} for {}", pwm, pwm_duty, gpio);
}

SimpleBackend::~SimpleBackend()
{
    spdlog::debug("SimpleBackend: Terminated.");
}

};

#endif
