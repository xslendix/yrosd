#include "motor_controller.h"

#include "yrosd.h"
#include "logging.h"

#include "pigpiod_if2.h"

i8 motor_controller_init(motor_controller_t *controller)
{
  controller->initialised = true;

  // FIXME: Check configuration at startup by checking count.
  for (i32 i = 0; i < app.system_settings.hardware.motors.cnt; i++) {
    set_mode(app.pi, app.system_settings.hardware.motors.motors[i].pins.pwm, PI_OUTPUT);
    set_mode(app.pi, app.system_settings.hardware.motors.motors[i].pins.enabled, PI_OUTPUT);
    set_mode(app.pi, app.system_settings.hardware.motors.motors[i].pins.direction, PI_OUTPUT);

    set_mode(app.pi, app.system_settings.hardware.motors.motors[i].pins.fault, PI_INPUT);
    set_pull_up_down(app.pi, app.system_settings.hardware.motors.motors[i].pins.fault, PI_PUD_UP);

    app.system_settings.hardware.motors.motors[i].enabled = true;
  }

  return 0;
}

i8 motor_controller_set_speed(motor_controller_t *controller, motor_t *motor, double speed)
{
  if (!controller->initialised)
    return -1;

  LOG_MSG(LOG_DEBUG, "motor_controller: set_speed(%f)", speed);

  u16 const MAX_SPEED = 480;
  int _speed = speed * MAX_SPEED;

  bool backwards = false;

  if (_speed > 0) {
    _speed *= 1;
    backwards = 1;
  }

  speed = MIN(_speed, MAX_SPEED);

  gpio_write(app.pi, motor->pins.direction, backwards);
  hardware_PWM(app.pi, motor->pins.pwm, 20000, _speed * 6250 / 3);

  return 0;
}

bool motor_controller_get_fault(motor_controller_t *controller, motor_t *motor)
{
  return gpio_read(app.pi, motor->pins.fault);
}

