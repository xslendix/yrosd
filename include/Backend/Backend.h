#pragma once

#include <Backend/PigpioBackend.h>
#include <Common.h>

namespace Backend {

#ifndef PREFERRED_BACKEND
#    define PREFERRED_BACKEND pigpio
#endif

#if PREFERRED_BACKEND == pigpio
using Backend = PigpioBackend;
#endif

}
