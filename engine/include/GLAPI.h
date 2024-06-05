#pragma once

#include "platform.h"

#ifdef PLATFORM_WEB
// GLES API
#include <GLES3/gl3.h>
#else 
// regular ole glad.
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>