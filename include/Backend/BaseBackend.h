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

#pragma region GPIO

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

#pragma endregion

#pragma region I2C

    virtual int OpenI2C(uint bus, uint address, uint flags)
    {
        spdlog::debug("Backend: I2C open requested: bus {}, address {}, flags {}.", bus, address, flags);
        return 0;
    }

    virtual bool CloseI2C(uint handle)
    {
        spdlog::debug("Backend: I2C close requested: handle {}.", handle);
        return true;
    }

    virtual u8 I2CReadByte(uint handle)
    {
        spdlog::debug("Backend: I2C read byte requested: handle {}.", handle);
        return 0;
    }

    virtual bool I2CWriteByte(uint handle, u8 data)
    {
        spdlog::debug("Backend: I2C write byte requested: handle {}, data {}.", handle, data);
        return true;
    }

    virtual uint I2CReadByteData(uint handle, uint reg)
    {
        spdlog::debug("Backend: I2C read byte data requested: handle {}, reg {}.", handle, reg);
        return 0;
    }

    virtual bool I2CWriteByteData(uint handle, uint reg, u8 data)
    {
        spdlog::debug("Backend: I2C write byte data requested: handle {}, reg {}, data {}.", handle, reg, data);
        return true;
    }

#pragma endregion

    virtual ~BaseBackend()
    {
        spdlog::debug("Backend: Terminating.");
    }
};

}

using Backend::GpioMode;
