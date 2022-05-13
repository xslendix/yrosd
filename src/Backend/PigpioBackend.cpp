#if PREFERRED_BACKEND == 2

#    include <Backend/PigpioBackend.h>

#    include <Common.h>
#    include <Signals.h>

#    include <pigpio.h>

namespace Backend {

void PigpioBackend::init()
{
    spdlog::debug("PigpioBackend: Initializing.");
    if (gpioInitialise() < 0) {
        spdlog::error("PigpioBackend: Failed to initialize.");
        clean_exit(1);
        return;
    }
    spdlog::debug("PigpioBackend: Initialized.");
}

void PigpioBackend::SetGPIOMode(u8 gpio, GpioMode mode)
{
    spdlog::debug("PigpioBackend: GPIO mode requested: {} for {}.", (uint)mode, gpio);
    gpioSetMode(gpio, (uint)mode);
}

bool PigpioBackend::GetGPIO(u8 gpio)
{
    spdlog::debug("PigpioBackend: GPIO value requested for {}.", gpio);
    return gpioRead(gpio);
}

void PigpioBackend::SetGPIO(u8 gpio, bool state)
{
    spdlog::debug("PigpioBackend: GPIO value change requested: {} for {}", state, gpio);
    gpioWrite(gpio, state);
}

void PigpioBackend::SetPullUpDown(u8 gpio, uint pud)
{
    spdlog::debug("PigpioBackend: GPIO pull up down change requested: {} for {}", pud, gpio);
    gpioSetPullUpDown(gpio, pud);
}

void PigpioBackend::SetHardwarePWM(u8 gpio, uint pwm, uint pwm_duty)
{
    spdlog::debug("PigpioBackend: GPIO hardware PWM change requested: {}, {} for {}", pwm, pwm_duty, gpio);
    gpioHardwarePWM(gpio, pwm, pwm_duty);
}

PigpioBackend::~PigpioBackend()
{
    spdlog::debug("PigpioBackend: Terminating.");
    gpioTerminate();
    spdlog::debug("PigpioBackend: Terminated.");
}

}

#endif
