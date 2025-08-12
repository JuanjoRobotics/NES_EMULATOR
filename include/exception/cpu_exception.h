#pragma once
#include <exception>
#include <string>

class cpu_exception : public std::exception
{
	std::string msg;

public:
	cpu_exception(const std::string &m, const char *file, int line, const char *func)
		: msg(std::string("[CPU EXCEPTION] ") + file + ":" + std::to_string(line) + " (" + func + ") - " + m)
	{
	}
	const char *what() const noexcept override { return msg.c_str(); }
};

// Macro for easy throwing with context
#define THROW_CPU_EXCEPTION(message) throw cpu_exception(message, __FILE__, __LINE__, __func__)