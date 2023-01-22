#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "common.h"
#include "src/types.h"

typedef struct motor_controller {
  bool initialised;
} motor_controller_t;

i8 motor_controller_init(motor_controller_t *controller);
i8 motor_controller_set_speed(motor_controller_t *controller, motor_t *motor, double speed);
bool motor_controller_get_fault(motor_controller_t *controller, motor_t *motor);

#endif // MOTOR_CONTROLLER_H

