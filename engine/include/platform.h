#pragma once

#ifdef __EMSCRIPTEN__
#define PLATFORM_WEB
#elif defined(_WIN32)
#define PLATFORM_WINDOWS
#elif defined(__unix__)
#define PLATFORM_LINUX
#elif defined(__APPLE__)
#define PLATFORM_MAC
#endif

#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)
#define PLATFORM_DESKTOP
#endif