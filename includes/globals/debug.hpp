#pragma once

#include <iostream>

#define LOG_LEVEL 5
/*
    0 : No logging
    1 : Error
    2 : Warn
    3 : Info
    4 : Debug
*/

#if LOG_LEVEL >= 5
#define LOG_DEBUG(source, output) \
    std::cout << "DEBUG: [" << source << "] " << output << '\n';
#else
#define LOG_DEBUG(source, output)
#endif

#if LOG_LEVEL >= 4
#define LOG_INFO(source, output) \
    std::cout << "INFO: [" << source << "] " << output << '\n';
#else
#define LOG_INFO(source, output)
#endif

#if LOG_LEVEL >= 3
#define LOG_WARN(source, output) \
    std::cerr << "WARN: [" << source << "] " << output << '\n';
#else
#define LOG_WARN(source, output)
#endif

#if LOG_LEVEL >= 2
#define LOG_ERROR(source, output) \
    std::cerr << "ERROR: [" << source << "] " << output << '\n';
#else
#define LOG_ERROR(source, output)
#endif

#if LOG_LEVEL >= 1
#define LOG_FATAL(source, output) \
    std::cerr << "FATAL: [" << source << "] " << output << '\n';
#else
#define LOG_FATAL(source, output)
#endif

