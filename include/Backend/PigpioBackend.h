#pragma once

#include <Backend/BaseBackend.h>
#include <Common.h>

#include <pigpio.h>

namespace Backend
{

class PigpioBackend : public BaseBackend
{
public:
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

    PigpioBackend();

    void init();
    void SetGPIOMode(u8 gpio, GpioMode mode);
    bool GetGPIO(u8 gpio);
    void SetGPIO(u8 gpio, bool state);
    void SetPullUpDown(u8 gpio, uint pud);
    void SetHardwarePWM(u8 gpio, uint pwm, uint pwm_duty);
    ~PigpioBackend();
};

}
