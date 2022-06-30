#ifndef EXTERNHEADER
#define EXTERNHEADER
#include <float.h>

#ifndef STM32
#define _USE_MATH_DEFINES
#else
#define M_PI           3.14159265358979323846  // Pi constant because not always included in all cmath header implementations
#endif

#include <math.h>
#endif
