// include/core/opcode.h
#pragma once
#include <string>
#include <cstdint>

class CPU;

enum class AddressingMode
{
	IMMEDIATE,
	ZERO_PAGE,
	ZERO_PAGE_X,
	ZERO_PAGE_Y,
	ABSOLUTE,
	ABSOLUTE_X,
	ABSOLUTE_Y,
	INDIRECT_X,
	INDIRECT_Y,
	NONE_ADDRESSING
};

using OpHandler = void (CPU::*)(AddressingMode);

struct Opcode
{
	OpHandler handler;
	const char *mnemonic;
	uint8_t length;
	uint8_t cycles;
	AddressingMode mode;
};

extern const Opcode OPCODES[256];