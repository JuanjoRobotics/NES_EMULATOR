#pragma once
#include <cstdint>
#include <vector>

#include "core/opcode.h"

class Bus;

class CPU
{
public:
	CPU();
	~CPU();

	void connect_bus(Bus *iBus) { bus = iBus; }

	void clock();
	void reset();
	void load(const std::vector<uint8_t> &program);
	void run();
	void irq();
	void nmi();
	void load_and_run(const std::vector<uint8_t> &program);

	// 6502 opcode handler methods
	void adc(AddressingMode mode);
	void and_op(AddressingMode mode);
	void asl(AddressingMode mode);
	void bcc(AddressingMode mode);
	void bcs(AddressingMode mode);
	void beq(AddressingMode mode);
	void bit(AddressingMode mode);
	void bmi(AddressingMode mode);
	void bne(AddressingMode mode);
	void bpl(AddressingMode mode);
	void bvc(AddressingMode mode);
	void bvs(AddressingMode mode);
	void clc(AddressingMode mode);
	void cld(AddressingMode mode);
	void cli(AddressingMode mode);
	void clv(AddressingMode mode);
	void cmp(AddressingMode mode);
	void cpx(AddressingMode mode);
	void cpy(AddressingMode mode);
	void dec(AddressingMode mode);
	void dex(AddressingMode mode);
	void dey(AddressingMode mode);
	void eor(AddressingMode mode);
	void inc(AddressingMode mode);
	void inx(AddressingMode mode);
	void iny(AddressingMode mode);
	void jmp(AddressingMode mode);
	void jsr(AddressingMode mode);
	void lda(AddressingMode mode);
	void ldx(AddressingMode mode);
	void ldy(AddressingMode mode);
	void lsr(AddressingMode mode);
	void ora(AddressingMode mode);
	void pha(AddressingMode mode);
	void php(AddressingMode mode);
	void pla(AddressingMode mode);
	void plp(AddressingMode mode);
	void rol(AddressingMode mode);
	void ror(AddressingMode mode);
	void rti(AddressingMode mode);
	void rts(AddressingMode mode);
	void sbc(AddressingMode mode);
	void sec(AddressingMode mode);
	void sed(AddressingMode mode);
	void sei(AddressingMode mode);
	void sta(AddressingMode mode);
	void stx(AddressingMode mode);
	void sty(AddressingMode mode);
	void tax(AddressingMode mode);
	void tay(AddressingMode mode);
	void tsx(AddressingMode mode);
	void txa(AddressingMode mode);
	void txs(AddressingMode mode);
	void tya(AddressingMode mode);

	enum FLAGS6502
	{
		CARRY = (1 << 0),
		ZERO = (1 << 1),
		INTERRUPT_DISABLE = (1 << 2),
		DECIMAL_MODE = (1 << 3),
		BREAK = (1 << 4),
		UNUSED = (1 << 5),
		OVERFLW = (1 << 6),
		SIGN = (1 << 7)
	};

	uint8_t a = 0x00;	   // Accumulator
	uint8_t x = 0x00;	   // X Register
	uint8_t y = 0x00;	   // Y Register
	uint16_t pc = 0x0000;  // Program Counter
	uint8_t sp = 0x00;	   // Stack Pointer
	uint8_t status = 0x00; // Status Register

private:
	Bus *bus = nullptr;
	uint8_t read(uint16_t address) const;
	void write(uint16_t address, uint8_t data);

	uint16_t read_u16(uint16_t address) const;
	void write_u16(uint16_t address, uint16_t data);

	uint8_t get_flag(FLAGS6502 flag) const;
	void set_flag(FLAGS6502 flag, bool set);

	uint16_t get_operand_address(AddressingMode mode) const;

	void set_accumulator(uint8_t value);
	void set_zero_flag(uint8_t value);
	void set_negative_flag(uint8_t value);
	void set_carry_flag();
	void clear_carry_flag();
};
