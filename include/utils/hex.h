#pragma once
#include <sstream>
#include <iomanip>
#include <string>

template <typename T>
std::string HEX(T value, int width = sizeof(T) * 2)
{
	std::ostringstream oss;
	oss << "0x" << std::uppercase << std::setfill('0') << std::setw(width) << std::hex << static_cast<uint64_t>(value);
	return oss.str();
}