#define CONFIG_CATCH_MAIN
#include "catch_amalgamated.hpp"
#include "core/cpu.h"
#include "core/bus.h"
#include "exception/cpu_exception.h"
#include <iostream>

/* LDA */
TEST_CASE("LDA opcode works with immediate addressing", "[opcode][lda][immediate]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xA9, 0x42, // LDA #$42
		0x00		// BRK
	};

	cpu.load_and_run(program);
	REQUIRE(cpu.get_accumulator() == 0x42);
}

TEST_CASE("LDA opcode works with zero page addressing", "[opcode][lda][zeropage]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xA5, 0x10, // LDA $10
		0x00		// BRK
	};

	bus.write(0x10, 0x15); // Set memory at $10 to 0x15
	cpu.load_and_run(program);
	REQUIRE(cpu.get_accumulator() == 0x15);
}

TEST_CASE("LDA opcode works with absolute addressing", "[opcode][lda][absolute]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xAD, 0x20, 0x00, // LDA $0020
		0x00			  // BRK
	};

	bus.write(0x0020, 0x99); // Set memory at $0020 to 0x99
	cpu.load_and_run(program);
	REQUIRE(cpu.get_accumulator() == 0x99);
}

TEST_CASE("LDA opcode works with zero page,X addressing", "[opcode][lda][zeropagex]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xA6, 0x10, // LDX $10
		0xB5, 0x20, // LDA $20,X
		0x00		// BRK
	};

	bus.write(0x10, 0x10);	 // Set memory at $10 to $10 (so X = $10)
	bus.write(0x0030, 0xAB); // Set memory at $20 + X (0x30) to 0xAB
	cpu.load_and_run(program);
	REQUIRE(cpu.get_accumulator() == 0xAB);
}

TEST_CASE("LDA opcode works with absolute,X addressing", "[opcode][lda][absolutex]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xA2, 0x10,		  // LDX #$10
		0xBD, 0x30, 0x00, // LDA $0030,X
		0x00			  // BRK
	};

	bus.write(0x0040, 0xCD); // Set memory at $0030 + X (0x0040) to 0xCD
	cpu.load_and_run(program);
	REQUIRE(cpu.get_accumulator() == 0xCD);
}

/* AND */
TEST_CASE("AND opcode works with immediate addressing", "[opcode][and][immediate]")
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
	REQUIRE(cpu.get_accumulator() == 0x02);
}

TEST_CASE("AND opcode works with zero page addressing", "[opcode][and][zeropage]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xA9, 0xF3, // LDA #$F3
		0x25, 0x10, // AND $10
		0x00		// BRK
	};
	bus.write(0x10, 0x0A); // $10 = 0x0A

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0x02);
}

TEST_CASE("AND opcode works with zero page,X addressing", "[opcode][and][zeropagex]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xA2, 0x01, // LDX #$01
		0xA9, 0xF3, // LDA #$F3
		0x35, 0x0F, // AND $0F,X (0x0F + X = 0x10)
		0x00		// BRK
	};
	bus.write(0x10, 0x0A); // $10 = 0x0A

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0x02);
}

TEST_CASE("AND opcode works with absolute addressing", "[opcode][and][absolute]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xA9, 0xF3,		  // LDA #$F3
		0x2D, 0x34, 0x12, // AND $1234
		0x00			  // BRK
	};
	bus.write(0x1234, 0x0A); // $1234 = 0x0A

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0x02);
}

TEST_CASE("AND opcode works with absolute,X addressing", "[opcode][and][absolutex]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xA2, 0x01,		  // LDX #$01
		0xA9, 0xF3,		  // LDA #$F3
		0x3D, 0x33, 0x12, // AND $1233,X (0x1233 + X = 0x1234)
		0x00			  // BRK
	};
	bus.write(0x1234, 0x0A); // $1234 = 0x0A

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0x02);
}

TEST_CASE("LDY opcode works with immediate addressing", "[opcode][ldy][immediate]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xA0, 0x55, // LDY #$55
		0x00		// BRK
	};

	cpu.load_and_run(program);
	REQUIRE(cpu.get_y() == 0x55);
}

TEST_CASE("LDY opcode works with zero page addressing", "[opcode][ldy][zeropage]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xA4, 0x10, // LDY $10
		0x00		// BRK
	};

	bus.write(0x10, 0x77); // Set memory at $10 to 0x77
	cpu.load_and_run(program);
	REQUIRE(cpu.get_y() == 0x77);
}

TEST_CASE("LDY opcode works with zero page,X addressing", "[opcode][ldy][zeropagex]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xA2, 0x05, // LDX #$05
		0xB4, 0x10, // LDY $10,X (0x10 + X = 0x15)
		0x00		// BRK
	};

	bus.write(0x15, 0x99); // Set memory at $15 to 0x99
	cpu.load_and_run(program);
	REQUIRE(cpu.get_y() == 0x99);
}

TEST_CASE("LDY opcode works with absolute addressing", "[opcode][ldy][absolute]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xAC, 0x34, 0x12, // LDY $1234
		0x00			  // BRK
	};

	bus.write(0x1234, 0x88); // Set memory at $1234 to 0x88
	cpu.load_and_run(program);
	REQUIRE(cpu.get_y() == 0x88);
}

TEST_CASE("LDY opcode works with absolute,X addressing", "[opcode][ldy][absolutex]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xA2, 0x01,		  // LDX #$01
		0xBC, 0x33, 0x12, // LDY $1233,X (0x1233 + X = 0x1234)
		0x00			  // BRK
	};

	bus.write(0x1234, 0xAA); // Set memory at $1234 to 0xAA
	cpu.load_and_run(program);
	REQUIRE(cpu.get_y() == 0xAA);
}

TEST_CASE("ADC opcode works with immediate addressing (no carry, no overflow)", "[opcode][adc][immediate]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	// LDA #$10, ADC #$22, BRK
	std::vector<uint8_t> program = {
		0xA9, 0x10, // LDA #$10
		0x69, 0x22, // ADC #$22
		0x00		// BRK
	};

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0x32); // 0x10 + 0x22 = 0x32
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::CARRY) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::ZERO) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::NEGATIVE) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::OVERFLW) == false);
}

TEST_CASE("ADC opcode sets carry flag on overflow (unNEGATIVEed)", "[opcode][adc][immediate][carry]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	// LDA #$F0, ADC #$20, BRK
	std::vector<uint8_t> program = {
		0xA9, 0xF0, // LDA #$F0
		0x69, 0x20, // ADC #$20
		0x00		// BRK
	};

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0x10); // 0xF0 + 0x20 = 0x110 -> 0x10
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::CARRY) == true);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::ZERO) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::NEGATIVE) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::OVERFLW) == false);
}

TEST_CASE("ADC opcode sets zero flag", "[opcode][adc][immediate][zero]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	// LDA #$00, ADC #$00, BRK
	std::vector<uint8_t> program = {
		0xA9, 0x00, // LDA #$00
		0x69, 0x00, // ADC #$00
		0x00		// BRK
	};

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0x00);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::ZERO) == true);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::CARRY) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::NEGATIVE) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::OVERFLW) == false);
}

TEST_CASE("ADC opcode sets negative flag", "[opcode][adc][immediate][negative]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	// LDA #$80, ADC #$01, BRK
	std::vector<uint8_t> program = {
		0xA9, 0x80, // LDA #$80
		0x69, 0x01, // ADC #$01
		0x00		// BRK
	};

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0x81);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::NEGATIVE) == true);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::ZERO) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::CARRY) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::OVERFLW) == false);
}

TEST_CASE("ADC opcode sets overflow flag (NEGATIVEed overflow)", "[opcode][adc][immediate][overflow]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	// LDA #$50, ADC #$50, BRK (80 + 80 = 160, which is -96 in NEGATIVEed 8-bit)
	std::vector<uint8_t> program = {
		0xA9, 0x50, // LDA #$50
		0x69, 0x50, // ADC #$50
		0x00		// BRK
	};

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0xA0);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::OVERFLW) == true);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::NEGATIVE) == true); // 0xA0 has high bit set
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::CARRY) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::ZERO) == false);
}

TEST_CASE("ADC opcode works with carry flag set", "[opcode][adc][immediate][carryin]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	// LDA #$01, CLC, ADC #$01, BRK (should be 2)
	std::vector<uint8_t> program = {
		0xA9, 0x01, // LDA #$01
		0x18,		// CLC (clear carry)
		0x69, 0x01, // ADC #$01
		0x00		// BRK
	};

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0x02);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::CARRY) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::ZERO) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::NEGATIVE) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::OVERFLW) == false);
}

TEST_CASE("ADC opcode works with carry flag set before ADC", "[opcode][adc][immediate][carryin]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	// LDA #$01, SEC, ADC #$01, BRK (should be 3)
	std::vector<uint8_t> program = {
		0xA9, 0x01, // LDA #$01
		0x38,		// SEC (set carry)
		0x69, 0x01, // ADC #$01
		0x00		// BRK
	};

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0x03);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::CARRY) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::ZERO) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::NEGATIVE) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::OVERFLW) == false);
}

/* SBC */
TEST_CASE("SBC opcode works with immediate addressing (no borrow, no overflow)", "[opcode][sbc][immediate]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	// LDA #$50, SEC, SBC #$20, BRK
	std::vector<uint8_t> program = {
		0xA9, 0x50, // LDA #$50
		0x38,		// SEC (set carry, so no borrow)
		0xE9, 0x20, // SBC #$20
		0x00		// BRK
	};

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0x30); // 0x50 - 0x20 = 0x30
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::CARRY) == true);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::ZERO) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::NEGATIVE) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::OVERFLW) == false);
}

TEST_CASE("SBC opcode sets borrow (carry clear) and negative flag", "[opcode][sbc][immediate][borrow]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	// LDA #$10, CLC, SBC #$20, BRK
	std::vector<uint8_t> program = {
		0xA9, 0x10, // LDA #$10
		0x18,		// CLC (clear carry, so borrow)
		0xE9, 0x20, // SBC #$20
		0x00		// BRK
	};

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0xEF); // 0x10 - 0x20 - 1 = 0xEF (underflow, wraps around)
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::CARRY) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::NEGATIVE) == true);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::ZERO) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::OVERFLW) == false);
}

TEST_CASE("SBC opcode sets zero flag", "[opcode][sbc][immediate][zero]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	// LDA #$01, SEC, SBC #$01, BRK
	std::vector<uint8_t> program = {
		0xA9, 0x01, // LDA #$01
		0x38,		// SEC
		0xE9, 0x01, // SBC #$01
		0x00		// BRK
	};

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0x00);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::ZERO) == true);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::CARRY) == true);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::NEGATIVE) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::OVERFLW) == false);
}

TEST_CASE("SBC opcode sets overflow flag (signed overflow)", "[opcode][sbc][immediate][overflow]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	// LDA #$80, SEC, SBC #$01, BRK (signed: -128 - 1 = 127, overflow)
	std::vector<uint8_t> program = {
		0xA9, 0x80, // LDA #$80
		0x38,		// SEC
		0xE9, 0x01, // SBC #$01
		0x00		// BRK
	};

	cpu.load_and_run(program);

	REQUIRE(cpu.get_accumulator() == 0x7F);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::OVERFLW) == true);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::NEGATIVE) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::ZERO) == false);
	REQUIRE(cpu.get_flag(CPU::FLAGS6502::CARRY) == true);
}

/* One test for each: LDX, LDY, LDA, STA, STX, STY */
TEST_CASE("LDX, LDY, LDA, STA, STX, STY opcodes basic functionality", "[opcode][loadstore]")
{
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	// LDX #$22, LDY #$33, LDA #$44, STA $10, STX $11, STY $12, BRK
	std::vector<uint8_t> program = {
		0xA2, 0x22, // LDX #$22
		0xA0, 0x33, // LDY #$33
		0xA9, 0x44, // LDA #$44
		0x85, 0x10, // STA $10
		0x86, 0x11, // STX $11
		0x84, 0x12, // STY $12
		0x00		// BRK
	};

	cpu.load_and_run(program);

	REQUIRE(cpu.get_x() == 0x22);
	REQUIRE(cpu.get_y() == 0x33);
	REQUIRE(cpu.get_accumulator() == 0x44);
	REQUIRE(bus.read(0x10) == 0x44);
	REQUIRE(bus.read(0x11) == 0x22);
	REQUIRE(bus.read(0x12) == 0x33);
}

/* INVALID */
TEST_CASE("Invalid Opcode should raise an exception", "[opcode][exception]")
{
	// Set up CPU and Bus
	Bus bus;
	CPU cpu;
	cpu.connect_bus(&bus);

	std::vector<uint8_t> program = {
		0xFF // Invalid opcode
	};

	// Expect an exception to be thrown
	REQUIRE_THROWS_AS(cpu.load_and_run(program), cpu_exception);
}