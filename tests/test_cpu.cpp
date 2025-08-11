#define CONFIG_CATCH_MAIN
#include "catch_amalgamated.hpp"
#include "core/cpu.h"
#include "core/bus.h"
#include <iostream>

TEST_CASE("LDA opcode works with different addressing modes", "[opcode][lda]")
{
	// Set up CPU and Bus
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	// Immediate
	std::vector<uint8_t> program = {
		0xA9, 0x42,
		0x00};

	cpu.load_and_run(program);
	REQUIRE(cpu.a == 0x42);

	// Zero page
	program = {
		0xA5, 0x10,
		0x00};

	bus.write(0x10, 0x15); // Set memory at $10 to 0x15
	cpu.load_and_run(program);
	REQUIRE(cpu.a == 0x15);

	// Absolute
	program = {
		0xAD, 0x20, 0x00,
		0x00};

	bus.write(0x0020, 0x99); // Set memory at $0020 to 0x99
	cpu.load_and_run(program);
	REQUIRE(cpu.a == 0x99);

	// Zero page X
	program = {
		0xA6, 0x10,
		0xB5, 0x20,
		0x00};

	bus.write(0x10, 0x10);	 // Set memory at $10 to $10
	bus.write(0x0030, 0xAB); // Set memory at $0020 + X (0x30) to 0xAB
	cpu.load_and_run(program);
	REQUIRE(cpu.a == 0xAB);

	// Absolute X
	/*
	program = {
		0xA2, 0x10,
		0xBD, 0x30, 0x00,
		0x00};

	bus.write(0x0040, 0xCD); // Set memory at $0030 + X (0x0040) to 0xCD
	cpu.load_and_run(program);
	REQUIRE(cpu.a == 0xCD);
	*/
}

TEST_CASE("AND opcode works with immediate addressing", "[opcode][and]")
{
	// Set up CPU and Bus
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xA9, 0xF3, // LDA #$F3
		0x29, 0x0A, // AND #$0A
		0x00		// BRK
	};

	cpu.load_and_run(program);

	// The result should be 0x02 (0xF3 & 0x0A)
	REQUIRE(cpu.a == 0x02);
}