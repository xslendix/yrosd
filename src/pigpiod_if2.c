#include "pigpiod_if2.h"

#include "common.h"
#include "logging.h"

u64 pin_values = 0;
u64 pin_modes = 0;
u64 pin_puds = 0;

int
pigpio_start(const char *addrStr, const char *portStr)
{
  LOG_MSG(LOG_DEBUG, "PigpioSim: pigpio_start(\"%s\", \"%s\")", addrStr, portStr);
  return 0;
}

void
pigpio_stop(int pi)
{
  LOG_MSG(LOG_DEBUG, "PigpioSim: pigpio_stop(%d)", pi);
}

int
gpio_read(int pi, unsigned gpio)
{
  LOG_MSG(LOG_DEBUG, "PigpioSim: gpio_read(%d, %d)", pi, gpio);
  return pin_values & (1 << gpio);
}

int
gpio_write(int pi, unsigned gpio, unsigned level)
{
  LOG_MSG(LOG_DEBUG, "PigpioSim: gpio_write(%d, %d, %d)", pi, gpio, level);
  pin_values |= (1 << gpio);
  return 0;
}

int
set_mode(int pi, unsigned gpio, unsigned mode)
{
  LOG_MSG(LOG_DEBUG, "PigpioSim: set_mode(%d, %d, %d)", pi, gpio, mode);
  pin_modes &= ~(1 << gpio);
  return 0;
}

int
set_pull_up_down(int pi, unsigned gpio, unsigned pud)
{
  LOG_MSG(LOG_DEBUG, "PigpioSim: set_pull_up_down(%d, %d, %d)", pi, gpio, pud);
  pin_puds &= ~(1 << gpio);
  return 0;
}

int
hardware_PWM(int pi, unsigned gpio, unsigned PWMfreq, u32 PWMduty)
{
  LOG_MSG(LOG_DEBUG, "PigpioSim: hardware_PWM(%d, %d, %d, %d)", pi, gpio, PWMfreq, PWMduty);
  return 0;
}
