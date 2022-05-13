#pragma once

#define BACKEND_SIMPLE 1
#define BACKEND_PIGPIO 2

#ifndef PREFERRED_BACKEND
#    define PREFERRED_BACKEND BACKEND_SIMPLE
#endif

#if PREFERRED_BACKEND == BACKEND_PIGPIO
#include <Backend/PigpioBackend.h>
#elif PREFERRED_BACKEND == BACKEND_SIMPLE
#include <Backend/SimpleBackend.h>
#endif
#include <Common.h>

namespace Backend {

#if PREFERRED_BACKEND == BACKEND_PIGPIO
using Backend = PigpioBackend;
#elif PREFERRED_BACKEND == BACKEND_SIMPLE
using Backend = SimpleBackend;
#endif

}
