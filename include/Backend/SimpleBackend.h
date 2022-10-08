#pragma once

#include <Backend/BaseBackend.h>
#include <Common.h>

#if PREFERRED_BACKEND == 1

namespace Backend {

class SimpleBackend : public BaseBackend {
public:
    SimpleBackend()
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

    int OpenI2C(uint bus, uint address, uint flags);
    bool CloseI2C(uint handle);
    u8 I2CReadByte(uint handle);
    bool I2CWriteByte(uint handle, u8 data);
    uint I2CReadByteData(uint handle, uint reg);
    bool I2CWriteByteData(uint handle, uint reg, u8 data);

    ~SimpleBackend();
};

}

#endif
