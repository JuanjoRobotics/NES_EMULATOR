#include <stdexcept>
#include <iostream>

#include "core/cpu.h"
#include "core/bus.h"

#include "debug.h"
#include "exception/cpu_exception.h"
#include "utils/hex.h"

CPU::CPU()
{
	a = 0x00;
	x = 0x00;
	y = 0x00;
	pc = 0x0000;
	sp = 0xFF;
	status = 0x00;
}

CPU::~CPU()
{
}

/* Memory access */
uint8_t CPU::read(uint16_t address) const
{
	DEBUG("Reading address: 0x" << HEX(address));
	return bus->read(address);
}

void CPU::write(uint16_t address, uint8_t data)
{
	DEBUG("Writing data: 0x" << HEX(data) << " to address: 0x" << HEX(address));
	bus->write(address, data);
}

uint16_t CPU::read_u16(uint16_t address) const
{
	uint16_t low = read(address);
	uint16_t high = read(address + 1);
	return (high << 8) | low;
}

void CPU::write_u16(uint16_t address, uint16_t data)
{
	write(address, data & 0xFF);			// Write low byte
	write(address + 1, (data >> 8) & 0xFF); // Write high byte
}

/* Utility */
uint16_t CPU::get_operand_address(AddressingMode mode) const
{
	switch (mode)
	{
	case AddressingMode::IMMEDIATE:
		return pc;
	case AddressingMode::ZERO_PAGE:
		return read(pc);
	case AddressingMode::ZERO_PAGE_X:
		return (read(pc) + x) & 0xFF;
	case AddressingMode::ZERO_PAGE_Y:
		return (read(pc) + y) & 0xFF;
	case AddressingMode::ABSOLUTE:
		return read_u16(pc);
	case AddressingMode::ABSOLUTE_X:
		return (read_u16(pc) + x) & 0xFFFF;
	case AddressingMode::ABSOLUTE_Y:
		return (read_u16(pc) + y) & 0xFFFF;
	case AddressingMode::INDIRECT_X:
	{
		uint16_t base = read(pc) + x;
		uint8_t low = read((base) & 0xFF);
		uint8_t high = read((base + 1) & 0xFF);
		uint16_t addr = (high << 8) | low;
		return addr;
	}
	case AddressingMode::INDIRECT_Y:
	{
		uint16_t base = read(pc);
		uint8_t low = read(base);
		uint8_t high = read((base + 1) & 0xFF);
		uint16_t addr = (high << 8) | low;
		return (addr + y) & 0xFFFF;
		return (addr + y) & 0xFFFF;
	}
	default:
		throw std::runtime_error(
			std::string("Unsupported or invalid addressing mode in get_operand_address at ") +
			__FILE__ + ":" + std::to_string(__LINE__));
	}
	DEBUG("Operand address for mode " << static_cast<int>(mode) << ": 0x" << HEX(get_operand_address(mode)));
}

/* Initialization */
void CPU::load(const std::vector<uint8_t> &program)
{
	uint16_t load_address = 0x8000;
	for (size_t i = 0; i < program.size(); ++i)
	{
		bus->write(load_address + i, program[i]);
	}
	write_u16(0xFFFC, 0x8000);
}

void CPU::reset()
{
	a = 0;
	x = 0;
	y = 0;
	status = 0b100100;
	pc = read_u16(0xFFFC);
}

void CPU::load_and_run(const std::vector<uint8_t> &program)
{
	load(program);
	reset();
	run();
}

/* logic */

void CPU::set_flag(FLAGS6502 flag, bool set)
{
	if (set)
		status |= static_cast<uint8_t>(flag);
	else
		status &= ~static_cast<uint8_t>(flag);
}

void CPU::set_negative_flag(uint8_t value)
{
	if (value == 0)
		set_flag(FLAGS6502::ZERO, true);
	else
		set_flag(FLAGS6502::ZERO, false);
}

void CPU::set_zero_flag(uint8_t value)
{
	if ((value & 0b1000'0000) != 0)
		set_flag(FLAGS6502::ZERO, true);
	else
		set_flag(FLAGS6502::ZERO, false);
}

void CPU::set_accumulator(uint8_t value)
{
	a = value;
	set_negative_flag(value);
	set_zero_flag(value);
}

/* opcodes */
void CPU::and_op(AddressingMode mode)
{
	uint16_t addr = get_operand_address(mode);
	uint8_t value = read(addr);

	set_accumulator(a & value);
}

void CPU::lda(AddressingMode mode)
{
	uint16_t addr = get_operand_address(mode);
	uint8_t value = read(addr);
	set_accumulator(value);
}

void CPU::ldx(AddressingMode mode)
{
	uint16_t addr = get_operand_address(mode);
	uint8_t value = read(addr);
	x = value;
	set_negative_flag(value);
	set_zero_flag(value);
}

void CPU::adc(AddressingMode mode) {}
void CPU::sbc(AddressingMode mode) {}
void CPU::asl(AddressingMode mode) {}
void CPU::lsr(AddressingMode mode) {}
void CPU::rol(AddressingMode mode) {}
void CPU::ror(AddressingMode mode) {}
void CPU::eor(AddressingMode mode) {}
void CPU::ora(AddressingMode mode) {}
void CPU::bit(AddressingMode mode) {}
void CPU::cmp(AddressingMode mode) {}
void CPU::cpx(AddressingMode mode) {}
void CPU::cpy(AddressingMode mode) {}
void CPU::inc(AddressingMode mode) {}
void CPU::inx(AddressingMode mode) {}
void CPU::iny(AddressingMode mode) {}
void CPU::dec(AddressingMode mode) {}
void CPU::dex(AddressingMode mode) {}
void CPU::dey(AddressingMode mode) {}
void CPU::jmp(AddressingMode mode) {}
void CPU::jsr(AddressingMode mode) {}
void CPU::rts(AddressingMode mode) {}
void CPU::rti(AddressingMode mode) {}
void CPU::bpl(AddressingMode mode) {}
void CPU::bmi(AddressingMode mode) {}
void CPU::bvc(AddressingMode mode) {}
void CPU::bvs(AddressingMode mode) {}
void CPU::bcc(AddressingMode mode) {}
void CPU::bcs(AddressingMode mode) {}
void CPU::bne(AddressingMode mode) {}
void CPU::beq(AddressingMode mode) {}
void CPU::clc(AddressingMode mode) {}
void CPU::sec(AddressingMode mode) {}
void CPU::cld(AddressingMode mode) {}
void CPU::sed(AddressingMode mode) {}
void CPU::cli(AddressingMode mode) {}
void CPU::sei(AddressingMode mode) {}
void CPU::clv(AddressingMode mode) {}
void CPU::tax(AddressingMode mode) {}
void CPU::txa(AddressingMode mode) {}
void CPU::tay(AddressingMode mode) {}
void CPU::tya(AddressingMode mode) {}
void CPU::tsx(AddressingMode mode) {}
void CPU::txs(AddressingMode mode) {}
void CPU::pha(AddressingMode mode) {}
void CPU::pla(AddressingMode mode) {}
void CPU::php(AddressingMode mode) {}
void CPU::plp(AddressingMode mode) {}
void CPU::ldy(AddressingMode mode) {}
void CPU::sta(AddressingMode mode) {}
void CPU::stx(AddressingMode mode) {}
void CPU::sty(AddressingMode mode) {}

void CPU::run()
{
	while (1)
	{
		uint8_t code = read(pc);
		pc++;
		uint16_t current_pc = pc;
		const Opcode &opcode = OPCODES[code];

		// Check if the handler exists
		if (opcode.handler)
		{
			// Call the handler function pointer
			(this->*opcode.handler)(opcode.mode);
		}
		else if (code == 0xEA)
		{ // NOP
			continue;
		}
		else if (code == 0x00)
		{ // BRK
			return;
		}
		else
		{
			THROW_CPU_EXCEPTION("Unsupported or invalid opcode");
		}

		// We only increment the program counter if the opcode is not a branch or jump
		if (current_pc == pc)
		{
			pc += opcode.length - 1;
		}
	}
}