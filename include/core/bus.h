#pragma once
#include <cstdint>
#include <array>

#include "core/cpu.h"

class Bus
{
public:
	Bus();
	~Bus();

	// Read and write
	void write(uint16_t address, uint8_t data);
	uint8_t read(uint16_t address, bool bReadOnly = false) const;

	// Devices on bus
	CPU cpu; // CPU instance
	std::array<uint8_t, 64 * 1024> memory;
};
