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
#define LOG_DEBUG(output) \
    std::cout << "DEBUG: [" << src << "] " << output << std::endl;
#else
#define LOG_DEBUG(output)
#endif

#if LOG_LEVEL >= 4
#define LOG_INFO(output) \
    std::cout << "INFO: [" << src << "] " << output << std::endl;
#else
#define LOG_INFO(output)
#endif

#if LOG_LEVEL >= 3
#define LOG_WARN(output) \
    std::cerr << "WARN: [" << src << "] " << output << std::endl;
#else
#define LOG_WARN(output)
#endif

#if LOG_LEVEL >= 2
#define LOG_ERROR(output) \
    std::cerr << "ERROR: [" << src << "] " << output << std::endl;
#else
#define LOG_ERROR(output)
#endif

#if LOG_LEVEL >= 1
#define LOG_FATAL(output) \
    std::cerr << "FATAL: [" << src << "] " << output << std::endl;
#else
#define LOG_FATAL(output)
#endif

