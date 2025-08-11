#include "core/bus.h"

Bus::Bus()
{
	// clear RAM contents
	for (auto &i : memory)
		i = 0x00;

	cpu.connect_bus(this);
}

Bus::~Bus()
{
}

void Bus::write(uint16_t address, uint8_t data)
{
	if (address < memory.size())
	{
		memory[address] = data;
	}
}

uint8_t Bus::read(uint16_t address, bool read_only) const
{
	if (address < memory.size())
	{
		return memory[address];
	}
	return 0x00; //  Out of bounds
}