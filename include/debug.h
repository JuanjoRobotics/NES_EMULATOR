
#pragma once
#include <iostream>

// Internal macro to print log messages with context
#define _LOG(level, msg)              \
	std::cout << "[" << level << "] " \
			  << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") - " << msg << std::endl

#ifdef NDEBUG
#define DEBUG(msg) \
	do             \
	{              \
	} while (0)
#else
#define DEBUG(msg) _LOG("DEBUG", msg)
#endif

#define INFO(msg) _LOG("INFO", msg)
#define WARN(msg) _LOG("WARN", msg)
#define ERROR(msg) _LOG("ERROR", msg)
