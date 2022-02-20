#pragma once

#include <Backend/BaseBackend.h>
#include <Common.h>

#include <pigpio.h>

namespace Backend {

class PigpioBackend : public BaseBackend {
public:
    PigpioBackend()
        : BaseBackend()
    {
        init();
    }

    void init();
    void SetGPIOMode(u8 gpio, GpioMode mode);
    bool GetGPIO(u8 gpio);
    void SetGPIO(u8 gpio, bool state);
    void SetPullUpDown(u8 gpio, uint pud);
    void SetHardwarePWM(u8 gpio, uint pwm, uint pwm_duty);
    ~PigpioBackend();
};

}
