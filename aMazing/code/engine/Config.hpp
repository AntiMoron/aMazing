#pragma once

//To enable the console or not.
#define ENABLE_CONSOLE
//To enable the debug option
#define OPEN_DEBUG_OPTION

#ifdef _WIN32
#define WINDOWS_OS
#else
#ifdef __linux
#define LINUX_OS
#else
#error "Operating system can't be checked."
#endif
#endif

#if (_MSC_VER ) < 1800
#error "Compiler is not supported."
#endif