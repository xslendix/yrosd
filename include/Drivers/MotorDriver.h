#pragma once

#include <Common.h>

namespace Driver {

class MotorDriver {
public:
    MotorDriver(uint pwm_pin, uint flt_pin, uint enable_pin, uint dir_pin);

    void SetSpeed(int speed);
    void SetEnable(bool enable);

    bool GetFault();

    ~MotorDriver();

private:
    u8 m_pwm, m_flt, m_en, m_dir;
};

}
