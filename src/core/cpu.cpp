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

/* Getter methods */
uint8_t CPU::get_accumulator() const { return a; }
uint8_t CPU::get_x() const { return x; }
uint8_t CPU::get_y() const { return y; }
uint16_t CPU::get_pc() const { return pc; }
uint8_t CPU::get_sp() const { return sp; }
uint8_t CPU::get_status() const { return status; }

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
uint8_t CPU::get_carry_flag() const
{
	return get_flag(FLAGS6502::CARRY) ? 1 : 0;
}

bool CPU::get_flag(FLAGS6502 flag) const
{
	return (status & static_cast<uint8_t>(flag)) != 0;
}

void CPU::set_flag(FLAGS6502 flag, bool set)
{
	if (set)
		status |= static_cast<uint8_t>(flag);
	else
		status &= ~static_cast<uint8_t>(flag);
}

void CPU::set_negative_flag(uint8_t value)
{
	set_flag(FLAGS6502::NEGATIVE, (value & 0x80) != 0);
}

void CPU::set_zero_flag(uint8_t value)
{
	set_flag(FLAGS6502::ZERO, value == 0);
}

void CPU::set_accumulator(uint8_t value)
{
	a = value;
	set_negative_flag(value);
	set_zero_flag(value);
}

void CPU::set_x(uint8_t value)
{
	x = value;
	set_negative_flag(value);
	set_zero_flag(value);
}
void CPU::set_y(uint8_t value)
{
	y = value;
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
	set_x(value);
}

void CPU::ldy(AddressingMode mode)
{
	uint16_t addr = get_operand_address(mode);
	uint8_t value = read(addr);
	set_y(value);
}
void CPU::adc(AddressingMode mode)
{
	uint16_t addr = get_operand_address(mode);
	uint8_t value = read(addr);

	uint16_t sum = static_cast<uint16_t>(a) + static_cast<uint16_t>(value) + get_carry_flag();
	uint8_t result = static_cast<uint8_t>(sum);

	bool overflow = (~(a ^ value) & (a ^ result) & 0x80) != 0;

	set_accumulator(result);
	set_flag(FLAGS6502::CARRY, sum > 0xFF);
	set_flag(FLAGS6502::OVERFLW, overflow);
}

void CPU::sbc(AddressingMode mode)
{
	uint16_t addr = get_operand_address(mode);
	uint8_t value = read(addr);

	uint16_t sum = static_cast<uint16_t>(a) - static_cast<uint16_t>(value) - (1 - get_carry_flag());
	uint8_t result = static_cast<uint8_t>(sum);

	bool carryFlag = (a >= (value + (1 - get_carry_flag())));
	bool overflow = ((a ^ result) & (a ^ value) & 0x80) != 0;

	set_accumulator(result);
	set_flag(FLAGS6502::CARRY, carryFlag);
	set_flag(FLAGS6502::OVERFLW, overflow);
}

void CPU::clc(AddressingMode mode)
{
	set_flag(FLAGS6502::CARRY, false);
}

void CPU::sec(AddressingMode mode)
{
	set_flag(FLAGS6502::CARRY, true);
}

void CPU::inc(AddressingMode mode)
{
	uint16_t addr = get_operand_address(mode);
	uint8_t value = read(addr);

	value++;
	write(addr, value);
	set_negative_flag(value);
	set_zero_flag(value);
}

void CPU::inx(AddressingMode mode)
{
	set_x(x + 1);
}

void CPU::iny(AddressingMode mode)
{
	set_y(y + 1);
}

void CPU::dec(AddressingMode mode)
{
	uint16_t addr = get_operand_address(mode);
	uint8_t value = read(addr);

	value--;
	write(addr, value);
	set_negative_flag(value);
	set_zero_flag(value);
}
void CPU::dex(AddressingMode mode)
{
	set_x(x - 1);
}
void CPU::dey(AddressingMode mode)
{
	set_y(y - 1);
}

void CPU::stx(AddressingMode mode)
{
	uint16_t addr = get_operand_address(mode);
	write(addr, x);
}

void CPU::sty(AddressingMode mode)
{
	uint16_t addr = get_operand_address(mode);
	write(addr, y);
}

void CPU::sta(AddressingMode mode)
{
	uint16_t addr = get_operand_address(mode);
	write(addr, a);
}

void CPU::tax(AddressingMode mode)
{
	set_x(a);
}

void CPU::txa(AddressingMode mode)
{
	set_accumulator(x);
}

void CPU::tay(AddressingMode mode)
{
	set_y(a);
}

void CPU::tya(AddressingMode mode)
{
	set_accumulator(y);
}
void CPU::tsx(AddressingMode mode)
{
	set_x(sp);
}

void CPU::txs(AddressingMode mode)
{
	sp = x;
}

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
void CPU::cld(AddressingMode mode) {}
void CPU::sed(AddressingMode mode) {}
void CPU::cli(AddressingMode mode) {}
void CPU::sei(AddressingMode mode) {}
void CPU::clv(AddressingMode mode) {}
void CPU::pha(AddressingMode mode) {}
void CPU::pla(AddressingMode mode) {}
void CPU::php(AddressingMode mode) {}
void CPU::plp(AddressingMode mode) {}

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