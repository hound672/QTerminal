#ifndef INCLUDES_H
#define INCLUDES_H

#include <qglobal.h>

#define U32_MAX_VALUE					0xFFFFFFFF

#define RETURN_ERROR()				return -__LINE__
#define RETURN_OK()						return 1

#define IS_ENV_SET(env)				!qgetenv(env).isEmpty()
#define IS_ENV_CMP(env, val)  qgetenv(env) == val

#define _DIM(array)	sizeof(array) / sizeof(array[0])

#endif // INCLUDES_H
