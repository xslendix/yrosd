#pragma once

#include <Common.h>

namespace Backend {

enum class GpioMode {
    Input = 0,
    Output = 1,
    Alt0 = 4,
    Alt1 = 5,
    Alt2 = 6,
    Alt3 = 7,
    Alt4 = 3,
    Alt5 = 2,
};

class BaseBackend {
public:
    BaseBackend()
    {
        init();
    }

    virtual void init()
    {
        spdlog::debug("Backend: Initializing.");
    }

    virtual void SetGPIOMode(u8 gpio, GpioMode mode)
    {
        spdlog::debug("Backend: GPIO mode requested: {} for {}.", (uint)mode, gpio);
    }

    virtual bool GetGPIO(u8 gpio)
    {
        spdlog::debug("Backend: GPIO value requested for {}.", gpio);
        return false;
    }

    virtual void SetGPIO(u8 gpio, bool state)
    {
        spdlog::debug("Backend: GPIO value change requested: {} for {}", state, gpio);
    }

    virtual void SetPullUpDown(u8 gpio, uint pud)
    {
        spdlog::debug("Backend: GPIO pull up down change requested: {} for {}", pud, gpio);
    }

    virtual void SetHardwarePWM(u8 gpio, uint pwm, uint pwm_duty)
    {
        spdlog::debug("Backend: GPIO hardware PWM change requested: {}, {} for {}", pwm, pwm_duty, gpio);
    }

    virtual ~BaseBackend()
    {
        spdlog::debug("Backend: Terminating.");
    }
};

}

using Backend::GpioMode;
