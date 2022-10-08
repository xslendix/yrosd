#if PREFERRED_BACKEND == 1

#  include <Backend/SimpleBackend.h>

#  include <Common.h>

namespace Backend {

void
SimpleBackend::init()
{
  spdlog::debug("SimpleBackend: Initialized.");
}

void
SimpleBackend::SetGPIOMode(u8 gpio, GpioMode mode)
{
  spdlog::debug("SimpleBackend: GPIO mode requested: {} for {}.", (uint)mode, gpio);
}

bool
SimpleBackend::GetGPIO(u8 gpio)
{
  spdlog::debug("SimpleBackend: GPIO value requested for {}.", gpio);
  return 0;
}

void
SimpleBackend::SetGPIO(u8 gpio, bool state)
{
  spdlog::debug("SimpleBackend: GPIO value change requested: {} for {}", state, gpio);
}

void
SimpleBackend::SetPullUpDown(u8 gpio, uint pud)
{
  spdlog::debug("SimpleBackend: GPIO pull up down change requested: {} for {}", pud, gpio);
}

void
SimpleBackend::SetHardwarePWM(u8 gpio, uint pwm, uint pwm_duty)
{
  spdlog::debug("SimpleBackend: GPIO hardware PWM change requested: {}, {} for {}", pwm, pwm_duty, gpio);
}

int
SimpleBackend::OpenI2C(uint bus, uint address, uint flags)
{
  spdlog::debug("SimpleBackend: I2C bus requested: {}, {}.", bus, address);
  return 0;
}
bool
SimpleBackend::CloseI2C(uint handle)
{
  spdlog::debug("SimpleBackend: I2C close requested: handle {}.", handle);
  return true;
}
u8
SimpleBackend::I2CReadByte(uint handle)
{
  spdlog::debug("SimpleBackend: I2C read byte requested: handle {}.", handle);
  return 0;
}
bool
SimpleBackend::I2CWriteByte(uint handle, u8 data)
{
  spdlog::debug("SimpleBackend: I2C write byte requested: handle {}, data {}.", handle, data);
  return true;
}
uint
SimpleBackend::I2CReadByteData(uint handle, uint reg)
{
  spdlog::debug("SimpleBackend: I2C read byte data requested: handle {}, reg {}.", handle, reg);
  return 0;
}
bool
SimpleBackend::I2CWriteByteData(uint handle, uint reg, u8 data)
{
  spdlog::debug("SimpleBackend: I2C write byte data requested: handle {}, reg {}, data {}.", handle, reg, data);
  return true;
}

SimpleBackend::~SimpleBackend()
{
  spdlog::debug("SimpleBackend: Terminated.");
}

};

#endif
