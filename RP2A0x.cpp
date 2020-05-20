#include <iostream>
#include <vector>

// Types Addressing Declaration

#define _ACCUMULATOR 0x00
#define _IMPLIED 0x01
#define _IMMEDIATE 0x02

#define _RELATIVE 0x03

#define _ABSOLUTE 0x04
#define _ABSOLUTE_X 0x05
#define _ABSOLUTE_Y 0x06

#define _ZERO_PAGE 0x07
#define _ZERO_PAGE_X 0x08
#define _ZERO_PAGE_Y 0x09

#define _INDIRECT 0x0A
#define _INDIRECT_X 0x0B
#define _INDIRECT_Y 0x0C

using namespace std;

// CPU Registers

uint8_t A = 0x00; // Accumulator
uint8_t X = 0x00; // Index Register X
uint8_t Y = 0x00; // Index Register Y
uint8_t S = 0xFF; // Stack Pointer

//CPU Flags

uint8_t C = 0x00; // Carry Flag
uint8_t Z = 0x00; // Zero Flag
uint8_t I = 0x00; // Interrupt Flag
uint8_t D = 0x00; // BCD Flag
uint8_t B = 0x00; // BRK Flag
uint8_t V = 0x00; // Overflow Flag
uint8_t N = 0x00; // Negative Flag

uint16_t PC = 0x0000; // Next Instruction Address 

//CPU clock counter

uint8_t clock = 0x00;

//CPU Internal Memory

extern vector<uint8_t> cpu_memory(UINT16_MAX); // Main Memory Bank

/*
$0000-$07FF RAM
$0800-$1FFF RAM Mirror (x3)
$2000-$2007 Registers Video
$2008-$3FFF Registrers Video Mirror (x1023)
$4000-$4017 Registers Audio & DMA & I/O
$4018-$4FFF Not used
$5000-$5FFF Expansion ROM (for MMC5)
$6000-$7FFF SRAM (WRAM)
$8000-$BFFF PRG-ROM (1)
$C000-$FFFF PRG-ROM (0)
*/

//Opcodes declaration

void _ADC(uint8_t type_addressing);
void _BCS();
void _BNE();
void _BVS();
void _CLV();
void _DEC(uint8_t type_addressing);
void _INC(uint8_t type_addressing);
void _JSR(uint8_t type_addressing);
void _LSR(uint8_t type_addressing);
void _PHP();
void _ROR(uint8_t type_addressing);
void _SEC();
void _STX(uint8_t type_addressing);
void _TSX();
void _AND(uint8_t type_addressing);
void _BEQ();
void _BPL();
void _CLC();
void _CMP(uint8_t type_addressing);
void _DEX();
void _INX();
void _LDA(uint8_t type_addressing);
void _NOP();
void _PLA(uint8_t type_addressing);
void _RTI();
void _SED();
void _STY(uint8_t type_addressing);
void _TXA();
void _ASL(uint8_t type_addressing);
void _BIT(uint8_t type_addressing);
void _BRK();
void _CLD();
void _CPX(uint8_t type_addressing);
void _DEY();
void _INY();
void _LDX(uint8_t type_addressing);
void _ORA(uint8_t type_addressing);
void _PLP();
void _RTS();
void _SEI();
void _TAX();
void _TXS();
void _BCC();
void _BMI();
void _BVC();
void _CLI();
void _CPY(uint8_t type_addressing);
void _EOR(uint8_t type_addressing);
void _JMP(uint8_t type_addressing);
void _LDY(uint8_t type_addressing);
void _PHA();
void _ROL(uint8_t type_addressing);
void _SBC(uint8_t type_addressing);
void _STA(uint8_t type_addressing);
void _TAY();
void _TYA();

// $FFFA NMI
// $FFFC RESET
// $FFFE IRQ/BRK

void nmi();
void reset();
void irq();

// Addressing Modes Implementation

void RELATIVE(uint16_t address)
{
	PC += (int8_t)cpu_memory[address + 1] + 1;
}

uint8_t* ABSOLUTE(void)
{
	uint16_t data = cpu_memory[PC + 2];
	data <<= 8;
	data |= cpu_memory[PC + 1];
	return &cpu_memory[data];
}

uint8_t* ABSOLUTE_X(void)
{
	uint16_t data = cpu_memory[PC + 2];
	data <<= 8;
	data |= cpu_memory[PC + 1];
	data += X;
	return &cpu_memory[data];
}

uint8_t* ABSOLUTE_Y(void)
{
	uint16_t data = cpu_memory[PC + 2];
	data <<= 8;
	data |= cpu_memory[PC + 1];
	data += Y;
	return &cpu_memory[data];
}

uint8_t* ZERO_PAGE_X(void)
{
	uint8_t data = cpu_memory[PC + 1];
	data += X;
	return &cpu_memory[data];
}

uint8_t* ZERO_PAGE_Y(void)
{
	uint8_t data = cpu_memory[PC + 1];
	data += Y;
	return &cpu_memory[data];
}

uint8_t* INDIRECT(void)
{
	uint16_t data = cpu_memory[PC + 2];
	data <<= 8;
	data |= cpu_memory[PC + 1];
	return &cpu_memory[data];
}

uint8_t* INDIRECT_X(void)
{
	uint16_t data = cpu_memory[PC + 2];
	data <<= 8;
	data |= cpu_memory[PC + 1];
	data += X;
	return &cpu_memory[data];
}

uint8_t* INDIRECT_Y(void)
{
	uint16_t data = cpu_memory[PC + 2];
	data <<= 8;
	data |= cpu_memory[PC + 1];
	data += Y;
	return &cpu_memory[data];
}

//Opcodes implementation

//Load Accumulator with Memory
void _LDA(uint8_t type_addressing)
{
	switch (type_addressing)
	{
	case _IMMEDIATE:
	{
		A = cpu_memory[PC + 1];
		PC += 2;

		break;
	}
	case _ZERO_PAGE:
	{
		A = cpu_memory[cpu_memory[PC + 1]];
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		A = *ZERO_PAGE_X();
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		A = *ABSOLUTE();
		PC += 3;
		break;
	}
	case _ABSOLUTE_X:
	{
		A = *ABSOLUTE_X();
		PC += 3;

		break;
	}
	case _ABSOLUTE_Y:
	{
		A = *ABSOLUTE_Y();
		PC += 3;

		break;
	}
	case _INDIRECT_X:
	{
		A = *INDIRECT_X();
		PC += 2;

		break;
	}
	case _INDIRECT_Y:
	{
		A = *INDIRECT_Y();
		PC += 2;

		break;
	}
	}

	//update flags

	Z = ((A == 0x00) ? 1 : 0);
	N = ((A >> 7) ? 1 : 0);
}

//Load Index X with Memory
void _LDX(uint8_t type_addressing)
{
	switch (type_addressing)
	{
	case _IMMEDIATE:
	{
		X = cpu_memory[PC + 1];
		PC += 2;

		break;
	}
	case _ZERO_PAGE:
	{
		X = cpu_memory[cpu_memory[PC + 1]];
		PC += 2;

		break;
	}
	case _ZERO_PAGE_Y:
	{
		X = *ZERO_PAGE_Y();
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		X = *ABSOLUTE();
		PC += 3;
		break;
	}
	case _ABSOLUTE_Y:
	{
		X = *ABSOLUTE_Y();
		PC += 3;

		break;
	}
	}

	//update flags

	Z = ((X == 0x00) ? 1 : 0);
	N = ((X >> 7) ? 1 : 0);

}

//Load Index Y with Memory
void _LDY(uint8_t type_addressing)
{
	switch (type_addressing)
	{
	case _IMMEDIATE:
	{
		Y = cpu_memory[PC + 1];
		PC += 2;

		break;
	}
	case _ZERO_PAGE:
	{
		Y = cpu_memory[cpu_memory[PC + 1]];
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		Y = *ZERO_PAGE_X();
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		Y = *ABSOLUTE();
		PC += 3;
		break;
	}
	case _ABSOLUTE_X:
	{
		Y = *ABSOLUTE_X();
		PC += 3;

		break;
	}
	}

	//update flags

	Z = ((Y == 0x00) ? 1 : 0);
	N = ((Y >> 7) ? 1 : 0);
}

//Store Accumulator in Memory
void _STA(uint8_t type_addressing)
{
	switch (type_addressing)
	{
	case _ZERO_PAGE:
	{
		(cpu_memory[cpu_memory[PC + 1]]) = A;
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		*(ZERO_PAGE_X()) = A;
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		*(ABSOLUTE()) = A;
		PC += 3;
		break;
	}
	case _ABSOLUTE_X:
	{
		*(ABSOLUTE_X()) = A;
		PC += 3;

		break;
	}
	case _ABSOLUTE_Y:
	{
		*(ABSOLUTE_Y()) = A;
		PC += 3;

		break;
	}
	case _INDIRECT_X:
	{
		*(INDIRECT_X()) = A;
		PC += 2;

		break;
	}
	case _INDIRECT_Y:
	{
		*(INDIRECT_Y()) = A;
		PC += 2;

		break;
	}
	}
}

//Store Index X in Memory
void _STX(uint8_t type_addressing)
{
	switch (type_addressing)
	{
	case _ZERO_PAGE:
	{
		(cpu_memory[cpu_memory[PC + 1]]) = X;
		PC += 2;

		break;
	}
	case _ZERO_PAGE_Y:
	{
		*(ZERO_PAGE_Y()) = X;
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		*(ABSOLUTE()) = X;
		PC += 3;
		break;
	}
	}
}

//Store Index Y in Memory
void _STY(uint8_t type_addressing)
{
	switch (type_addressing)
	{
	case _ZERO_PAGE:
	{
		(cpu_memory[cpu_memory[PC + 1]]) = Y;
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		*(ZERO_PAGE_X()) = Y;
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		*(ABSOLUTE()) = Y;
		PC += 3;
		break;
	}
	}
}

//Transfer Accumulator to Index X
void _TAX()
{
	X = A;

	// update flags

	Z = ((X == 0x00) ? 1 : 0);
	N = ((X >> 7) ? 1 : 0);
}

//Transfer Accumulator to Index Y 
void _TAY()
{
	Y = A;

	// update flags

	Z = ((X == 0x00) ? 1 : 0);
	N = ((X >> 7) ? 1 : 0);
}

//Transfer Index X to Accumulator
void _TXA()
{
	A = X;

	// update flags

	Z = ((A == 0x00) ? 1 : 0);
	N = ((A >> 7) ? 1 : 0);
}

//Transfer Index Y to Accumulator
void _TYA()
{
	A = Y;

	// update flags

	Z = ((A == 0x00) ? 1 : 0);
	N = ((A >> 7) ? 1 : 0);
}

//Transfer Stack Pointer to Index X
void _TSX()
{
	X = S;

	// update flags

	Z = ((X == 0x00) ? 1 : 0);
	N = ((X >> 7) ? 1 : 0);
}

//Transfer Index X to Stack Pointer
void _TXS()
{
	S = X;

	// update flags

	Z = ((S == 0x00) ? 1 : 0);
	N = ((S >> 7) ? 1 : 0);
}

//Push Accumulator on Stack
void _PHA()
{
	uint16_t stack_full_address = 0x0100 + S;
	cpu_memory[stack_full_address] = A;
	S--;
	PC++;
}

//Push Processor Status on Stack
void _PHP()
{
	uint16_t stack_full_address = 0x0100 + S;

	cpu_memory[stack_full_address] = N;
	cpu_memory[stack_full_address] <<= 1;
	cpu_memory[stack_full_address] |= V;
	cpu_memory[stack_full_address] <<= 2;
	cpu_memory[stack_full_address] |= B;
	cpu_memory[stack_full_address] <<= 1;
	cpu_memory[stack_full_address] |= D;
	cpu_memory[stack_full_address] <<= 1;
	cpu_memory[stack_full_address] |= I;
	cpu_memory[stack_full_address] <<= 1;
	cpu_memory[stack_full_address] |= Z;
	cpu_memory[stack_full_address] <<= 1;
	cpu_memory[stack_full_address] |= C;

	S--;
	PC++;
}

//Pull Accumulator from Stack
void _PLA(uint8_t type_addressing)
{
	uint16_t stack_full_address = 0x0100 + S;
	A = cpu_memory[stack_full_address];
	S++;
	PC++;
}

//Pull Processor Status from Stack
void _PLP()
{
	uint16_t stack_full_address = 0x0100 + S;

	N = (cpu_memory[stack_full_address] & 0b10000000) >> 7;
	V = (cpu_memory[stack_full_address] & 0b01000000) >> 6;
	B = (cpu_memory[stack_full_address] & 0b00010000) >> 4;
	D = (cpu_memory[stack_full_address] & 0b00001000) >> 3;
	I = (cpu_memory[stack_full_address] & 0b00000100) >> 2;
	Z = (cpu_memory[stack_full_address] & 0b00000010) >> 1;
	C = (cpu_memory[stack_full_address] & 0b00000001);

	S++;
	PC++;
}

//"AND" Memory with Accumulator
void _AND(uint8_t type_addressing)
{
	switch (type_addressing)
	{
	case _IMMEDIATE:
	{
		A &= cpu_memory[PC + 1];
		PC += 2;

		break;
	}
	case _ZERO_PAGE:
	{
		A &= cpu_memory[cpu_memory[PC + 1]];
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		A &= *ZERO_PAGE_X();
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		A &= *ABSOLUTE();
		PC += 3;
		break;
	}
	case _ABSOLUTE_X:
	{
		A &= *ABSOLUTE_X();
		PC += 3;

		break;
	}
	case _ABSOLUTE_Y:
	{
		A &= *ABSOLUTE_Y();
		PC += 3;

		break;
	}
	case _INDIRECT_X:
	{
		A &= *INDIRECT_X();
		PC += 2;

		break;
	}
	case _INDIRECT_Y:
	{
		A &= *INDIRECT_Y();
		PC += 2;

		break;
	}
	}

	//update flags

	Z = ((A == 0x00) ? 1 : 0);
	N = ((A >> 7) ? 1 : 0);
}

//"Exclusive-Or" Memory with Accumulator
void _EOR(uint8_t type_addressing)
{
	switch (type_addressing)
	{
	case _IMMEDIATE:
	{
		A ^= cpu_memory[PC + 1];
		PC += 2;

		break;
	}
	case _ZERO_PAGE:
	{
		A ^= cpu_memory[cpu_memory[PC + 1]];
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		A ^= *ZERO_PAGE_X();
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		A ^= *ABSOLUTE();
		PC += 3;
		break;
	}
	case _ABSOLUTE_X:
	{
		A ^= *ABSOLUTE_X();
		PC += 3;

		break;
	}
	case _ABSOLUTE_Y:
	{
		A ^= *ABSOLUTE_Y();
		PC += 3;

		break;
	}
	case _INDIRECT_X:
	{
		A ^= *INDIRECT_X();
		PC += 2;

		break;
	}
	case _INDIRECT_Y:
	{
		A ^= *INDIRECT_Y();
		PC += 2;

		break;
	}
	}

	//update flags

	Z = ((A == 0x00) ? 1 : 0);
	N = ((A >> 7) ? 1 : 0);
}

//"OR" Memory with Accumulator
void _ORA(uint8_t type_addressing)
{
	switch (type_addressing)
	{
	case _IMMEDIATE:
	{
		A |= cpu_memory[PC + 1];
		PC += 2;

		break;
	}
	case _ZERO_PAGE:
	{
		A |= cpu_memory[cpu_memory[PC + 1]];
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		A |= *ZERO_PAGE_X();
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		A |= *ABSOLUTE();
		PC += 3;
		break;
	}
	case _ABSOLUTE_X:
	{
		A |= *ABSOLUTE_X();
		PC += 3;

		break;
	}
	case _ABSOLUTE_Y:
	{
		A |= *ABSOLUTE_Y();
		PC += 3;

		break;
	}
	case _INDIRECT_X:
	{
		A |= *INDIRECT_X();
		PC += 2;

		break;
	}
	case _INDIRECT_Y:
	{
		A |= *INDIRECT_Y();
		PC += 2;

		break;
	}
	}

	//update flags

	Z = ((A == 0x00) ? 1 : 0);
	N = ((A >> 7) ? 1 : 0);
}

//Test Bits in Memory with Accumulator
void _BIT(uint8_t type_addressing)
{
	switch (type_addressing)
	{
	case _ZERO_PAGE:
	{
		A &= cpu_memory[cpu_memory[PC + 1]];
		N = (cpu_memory[cpu_memory[PC + 1]] >> 7);
		V = ((cpu_memory[cpu_memory[PC + 1]] >> 6) & 0x1);

		break;
	}
	case _ABSOLUTE:
	{
		A &= *ABSOLUTE();
		N = (*ABSOLUTE() >> 7);
		V = ((*ABSOLUTE() >> 6) & 0x1);

		break;
	}
	}

	// update flags

	Z = ((A == 0x00) ? 1 : 0);
}

//Add Memory to Accumulator with Carry
void _ADC(uint8_t type_addressing)
{
	uint16_t data = 0x00;

	switch (type_addressing)
	{
	case _IMMEDIATE:
	{
		data = A + C + cpu_memory[PC + 1];
		V = ~((A ^ cpu_memory[PC + 1]) & 0x80) && ((A ^ cpu_memory[PC + 1]) & 0x80);
		A = data;
		PC += 2;

		break;
	}
	case _ZERO_PAGE:
	{
		data = A + C + cpu_memory[cpu_memory[PC + 1]];
		V = ~((A ^ cpu_memory[cpu_memory[PC + 1]]) & 0x80) && ((A ^ cpu_memory[cpu_memory[PC + 1]]) & 0x80);
		A = data;
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		data = A + C + *ZERO_PAGE_X();
		V = ~((A ^ *ZERO_PAGE_X()) & 0x80) && ((A ^ *ZERO_PAGE_X()) & 0x80);
		A = data;
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		data = A + C + *ABSOLUTE();
		V = ~((A ^ *ABSOLUTE()) & 0x80) && ((A ^ *ABSOLUTE()) & 0x80);
		A = data;
		PC += 3;

		break;
	}
	case _ABSOLUTE_X:
	{
		data = A + C + *ABSOLUTE_X();
		V = ~((A ^ *ABSOLUTE_X()) & 0x80) && ((A ^ *ABSOLUTE_X()) & 0x80);
		A = data;
		PC += 3;

		break;
	}
	case _ABSOLUTE_Y:
	{
		data = A + C + *ABSOLUTE_Y();
		V = ~((A ^ *ABSOLUTE_Y()) & 0x80) && ((A ^ *ABSOLUTE_Y()) & 0x80);
		A = data;
		PC += 3;

		break;
	}
	case _INDIRECT_X:
	{
		data = A + C + *INDIRECT_X();
		V = ~((A ^ *INDIRECT_X()) & 0x80) && ((A ^ *INDIRECT_X()) & 0x80);
		A = data;
		PC += 2;

		break;
	}
	case _INDIRECT_Y:
	{
		data = A + C + *INDIRECT_Y();
		V = ~((A ^ *INDIRECT_Y()) & 0x80) && ((A ^ *INDIRECT_Y()) & 0x80);
		A = data;
		PC += 2;

		break;
	}
	}

	//update flags
	C = ((data & 0x100) ? 1 : 0);
	Z = ((A == 0x00) ? 1 : 0);
	N = ((A >> 7) ? 1 : 0);
}

//Subtract Memory from Accumulator with Borrow
void _SBC(uint8_t type_addressing)
{
	uint16_t data = 0x00;

	switch (type_addressing)
	{
	case _IMMEDIATE:
	{
		data = A - ((C != 0) ? 0 : 1) - cpu_memory[PC + 1];
		V = ((A ^ cpu_memory[PC + 1]) & 0x80) && ((A ^ cpu_memory[PC + 1]) & 0x80);
		A = data;
		PC += 2;

		break;
	}
	case _ZERO_PAGE:
	{
		data = A - ((C != 0) ? 0 : 1) - cpu_memory[cpu_memory[PC + 1]];
		V = ((A ^ cpu_memory[cpu_memory[PC + 1]]) & 0x80) && ((A ^ cpu_memory[cpu_memory[PC + 1]]) & 0x80);
		A = data;
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		data = A - ((C != 0) ? 0 : 1) - *ZERO_PAGE_X();
		V = ((A ^ *ZERO_PAGE_X()) & 0x80) && ((A ^ *ZERO_PAGE_X()) & 0x80);
		A = data;
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		data = A - ((C != 0) ? 0 : 1) - *ABSOLUTE();
		V = ((A ^ *ABSOLUTE()) & 0x80) && ((A ^ *ABSOLUTE()) & 0x80);
		A = data;
		PC += 3;

		break;
	}
	case _ABSOLUTE_X:
	{
		data = A - ((C != 0) ? 0 : 1) - *ABSOLUTE_X();
		V = ((A ^ *ABSOLUTE_X()) & 0x80) && ((A ^ *ABSOLUTE_X()) & 0x80);
		A = data;
		PC += 3;

		break;
	}
	case _ABSOLUTE_Y:
	{
		data = A - ((C != 0) ? 0 : 1) - *ABSOLUTE_Y();
		V = ((A ^ *ABSOLUTE_Y()) & 0x80) && ((A ^ *ABSOLUTE_Y()) & 0x80);
		A = data;
		PC += 3;

		break;
	}
	case _INDIRECT_X:
	{
		data = A - ((C != 0) ? 0 : 1) - *INDIRECT_X();
		V = ((A ^ *INDIRECT_X()) & 0x80) && ((A ^ *INDIRECT_X()) & 0x80);
		A = data;
		PC += 2;

		break;
	}
	case _INDIRECT_Y:
	{
		data = A - ((C != 0) ? 0 : 1) - *INDIRECT_Y();
		V = ((A ^ *INDIRECT_Y()) & 0x80) && ((A ^ *INDIRECT_Y()) & 0x80);
		A = data;
		PC += 2;

		break;
	}
	}

	//update flags
	C = ((data < 0x100) ? 1 : 0);
	Z = ((A == 0x00) ? 1 : 0);
	N = ((A >> 7) ? 1 : 0);
}

//Compare Memory and Accumulator
void _CMP(uint8_t type_addressing)
{
	uint16_t data = 0x00;

	switch (type_addressing)
	{
	case _IMMEDIATE:
	{
		data = A - cpu_memory[PC + 1];
		V = ((A ^ cpu_memory[PC + 1]) & 0x80) && ((A ^ cpu_memory[PC + 1]) & 0x80);
		PC += 2;

		break;
	}
	case _ZERO_PAGE:
	{
		data = A - cpu_memory[cpu_memory[PC + 1]];
		V = ((A ^ cpu_memory[cpu_memory[PC + 1]]) & 0x80) && ((A ^ cpu_memory[cpu_memory[PC + 1]]) & 0x80);
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		data = A  - *ZERO_PAGE_X();
		V = ((A ^ *ZERO_PAGE_X()) & 0x80) && ((A ^ *ZERO_PAGE_X()) & 0x80);
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		data = A - *ABSOLUTE();
		V = ((A ^ *ABSOLUTE()) & 0x80) && ((A ^ *ABSOLUTE()) & 0x80);
		PC += 3;

		break;
	}
	case _ABSOLUTE_X:
	{
		data = A - *ABSOLUTE_X();
		V = ((A ^ *ABSOLUTE_X()) & 0x80) && ((A ^ *ABSOLUTE_X()) & 0x80);
		PC += 3;

		break;
	}
	case _ABSOLUTE_Y:
	{
		data = A - *ABSOLUTE_Y();
		V = ((A ^ *ABSOLUTE_Y()) & 0x80) && ((A ^ *ABSOLUTE_Y()) & 0x80);
		PC += 3;

		break;
	}
	case _INDIRECT_X:
	{
		data = A - *INDIRECT_X();
		V = ((A ^ *INDIRECT_X()) & 0x80) && ((A ^ *INDIRECT_X()) & 0x80);
		PC += 2;

		break;
	}
	case _INDIRECT_Y:
	{
		data = A - *INDIRECT_Y();
		V = ((A ^ *INDIRECT_Y()) & 0x80) && ((A ^ *INDIRECT_Y()) & 0x80);
		PC += 2;

		break;
	}
	}

	//update flags
	C = ((data < 0x100) ? 1 : 0);
	Z = ((data == 0x00) ? 1 : 0);
	N = ((data >> 7) ? 1 : 0);
}

//Compare Memory and Index X
void _CPX(uint8_t type_addressing)
{
	uint16_t data = 0x00;

	switch (type_addressing)
	{
	case _IMMEDIATE:
	{
		data = X - cpu_memory[PC + 1];
		V = ((A ^ cpu_memory[PC + 1]) & 0x80) && ((A ^ cpu_memory[PC + 1]) & 0x80);
		PC += 2;

		break;
	}
	case _ZERO_PAGE:
	{
		data = X - cpu_memory[cpu_memory[PC + 1]];
		V = ((A ^ cpu_memory[cpu_memory[PC + 1]]) & 0x80) && ((A ^ cpu_memory[cpu_memory[PC + 1]]) & 0x80);
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		data = X - *ABSOLUTE();
		V = ((A ^ *ABSOLUTE()) & 0x80) && ((A ^ *ABSOLUTE()) & 0x80);
		PC += 3;

		break;
	}
	}

	//update flags
	C = ((data < 0x100) ? 1 : 0);
	Z = ((data == 0x00) ? 1 : 0);
	N = ((data >> 7) ? 1 : 0);
}

//Compare Memory and Index Y
void _CPY(uint8_t type_addressing)
{
	uint16_t data = 0x00;

	switch (type_addressing)
	{
	case _IMMEDIATE:
	{
		data = Y - cpu_memory[PC + 1];
		V = ((A ^ cpu_memory[PC + 1]) & 0x80) && ((A ^ cpu_memory[PC + 1]) & 0x80);
		Y = data;
		PC += 2;

		break;
	}
	case _ZERO_PAGE:
	{
		data = Y - cpu_memory[cpu_memory[PC + 1]];
		V = ((A ^ cpu_memory[cpu_memory[PC + 1]]) & 0x80) && ((A ^ cpu_memory[cpu_memory[PC + 1]]) & 0x80);
		Y = data;
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		data = Y - *ABSOLUTE();
		V = ((A ^ *ABSOLUTE()) & 0x80) && ((A ^ *ABSOLUTE()) & 0x80);
		Y = data;
		PC += 3;

		break;
	}
	}

	//update flags
	C = ((data < 0x100) ? 1 : 0);
	Z = ((Y == 0x00) ? 1 : 0);
	N = ((Y >> 7) ? 1 : 0);
}

//Increment Memory by One
void _INC(uint8_t type_addressing)
{
	uint8_t data = 0x00;

	switch (type_addressing)
	{
	case _ZERO_PAGE:
	{
		data = (cpu_memory[cpu_memory[PC + 1]]) += 1;
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		data = *(ZERO_PAGE_X()) += 1;
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		data = *(ABSOLUTE()) += 1;
		PC += 3;
		break;
	}
	case _ABSOLUTE_X:
	{
		data = *(ABSOLUTE_X()) += 1;
		PC += 3;
		break;
	}
	}

	// update flags

	Z = ((data == 0x00) ? 1 : 0);
	N = ((data >> 7) ? 1 : 0);
}

//Increment Index X by One
void _INX()
{
	X++;

	// update flags

	Z = ((X == 0x00) ? 1 : 0);
	N = ((X >> 7) ? 1 : 0);
}

//Increment Index Y by One
void _INY()
{
	Y++;

	// update flags

	Z = ((Y == 0x00) ? 1 : 0);
	N = ((Y >> 7) ? 1 : 0);
}

//Decrement Memory by One
void _DEC(uint8_t type_addressing)
{
	uint8_t data = 0x00;

	switch (type_addressing)
	{
	case _ZERO_PAGE:
	{
		data = (cpu_memory[cpu_memory[PC + 1]]) -= 1;
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		data = *(ZERO_PAGE_X()) -= 1;
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		data = *(ABSOLUTE()) -= 1;
		PC += 3;
		break;
	}
	case _ABSOLUTE_X:
	{
		data = *(ABSOLUTE_X()) -= 1;
		PC += 3;
		break;
	}
	}

	// update flags

	Z = ((data == 0x00) ? 1 : 0);
	N = ((data >> 7) ? 1 : 0);
}

//Decrement Index X by One
void _DEX()
{
	X--;

	// update flags

	Z = ((X == 0x00) ? 1 : 0);
	N = ((X >> 7) ? 1 : 0);
}

//Decrement Index Y by One
void _DEY()
{
	Y--;

	// update flags

	Z = ((Y == 0x00) ? 1 : 0);
	N = ((Y >> 7) ? 1 : 0);
}

//Shift Left One Bit(Memory or Accumulator)
void _ASL(uint8_t type_addressing)
{
	uint8_t data = 0x00;

	switch (type_addressing)
	{
	case _ACCUMULATOR:
	{
		C = ((A & 0x80) >> 7);
		data = A <<= 1;
		PC += 1;

		break;
	}
	case _ZERO_PAGE:
	{
		C = ((cpu_memory[cpu_memory[PC + 1]] & 0x80) >> 7);
		data = cpu_memory[cpu_memory[PC + 1]] <<= 1;
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		C = ((*ZERO_PAGE_X() & 0x80) >> 7);
		data = *ZERO_PAGE_X() <<= 1;
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		*ABSOLUTE() = ((*ABSOLUTE() & 0x80) >> 7);
		data = *ABSOLUTE() <<= 1;
		PC += 3;

		break;
	}
	case _ABSOLUTE_X:
	{
		C = ((*ABSOLUTE_X() & 0x80) >> 7);
		data = *ABSOLUTE_X() <<= 1;
		PC += 3;

		break;
	}
	}

	// update flags

	Z = ((data == 0x00) ? 1 : 0);
	N = ((data >> 7) ? 1 : 0);
}

//Shift Right One Bit (Memory or Accumulator)
void _LSR(uint8_t type_addressing)
{
	uint8_t data = 0x00;

	switch (type_addressing)
	{
	case _ACCUMULATOR:
	{
		C = ((A & 0x01) << 7);
		data = A >>= 1;
		C >>= 7;
		PC += 1;

		break;
	}
	case _ZERO_PAGE:
	{
		C = ((cpu_memory[cpu_memory[PC + 1]] & 0x01) << 7);
		data = cpu_memory[cpu_memory[PC + 1]] >>= 1;
		C >>= 7;
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		C = ((*ZERO_PAGE_X() & 0x01) << 7);
		data = *ZERO_PAGE_X() >>= 1;
		C >>= 7;
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		C = ((*ABSOLUTE() & 0x01) << 7);
		data = *ABSOLUTE() >>= 1;
		C >>= 7;
		PC += 3;

		break;
	}
	case _ABSOLUTE_X:
	{
		C = ((*ABSOLUTE_X() & 0x01) << 7);
		data = *ABSOLUTE_X() >>= 1;
		C >>= 7;
		PC += 3;

		break;
	}
	}

	// update flags

	Z = ((data == 0x00) ? 1 : 0);
}

//Rotate One Bit Left (Memory or Accumulator)
void _ROL(uint8_t type_addressing)
{
	uint8_t data = 0x00;

	switch (type_addressing)
	{
	case _ACCUMULATOR:
	{
		C = ((A & 0x80) >> 7);
		A <<= 1;
		A |= C;
		PC += 1;

		break;
	}
	case _ZERO_PAGE:
	{
		C = ((cpu_memory[cpu_memory[PC + 1]] & 0x80) >> 7);
		cpu_memory[cpu_memory[PC + 1]] <<= 1;
		data = cpu_memory[cpu_memory[PC + 1]] |= C;
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		C = ((*ZERO_PAGE_X() & 0x80) >> 7);
		*ZERO_PAGE_X() <<= 1;
		data = *ZERO_PAGE_X() |= C;
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		*ABSOLUTE() = ((*ABSOLUTE() & 0x80) >> 7);
		*ABSOLUTE() <<= 1;
		data = *ABSOLUTE() |= C;
		PC += 3;

		break;
	}
	case _ABSOLUTE_X:
	{
		C = ((*ABSOLUTE_X() & 0x80) >> 7);
		*ABSOLUTE_X() <<= 1;
		data = *ABSOLUTE_X() |= C;
		PC += 3;

		break;
	}
	}

	// update flags

	Z = ((data == 0x00) ? 1 : 0);
	N = ((data >> 7) ? 1 : 0);
}

//Rotate One Bit Right (Memory or Accumulator)
void _ROR(uint8_t type_addressing)
{
	uint8_t data = 0x00;

	switch (type_addressing)
	{
	case _ACCUMULATOR:
	{
		C = ((A & 0x01) << 7);
		A >>= 1;
		data = A |= C;
		C >>= 7;
		PC += 1;

		break;
	}
	case _ZERO_PAGE:
	{
		C = ((cpu_memory[cpu_memory[PC + 1]] & 0x01) << 7);
		cpu_memory[cpu_memory[PC + 1]] >>= 1;
		data = cpu_memory[cpu_memory[PC + 1]] |= C;
		C >>= 7;
		PC += 2;

		break;
	}
	case _ZERO_PAGE_X:
	{
		C = ((*ZERO_PAGE_X() & 0x01) << 7);
		*ZERO_PAGE_X() >>= 1;
		data = *ZERO_PAGE_X() |= C;
		C >>= 7;
		PC += 2;

		break;
	}
	case _ABSOLUTE:
	{
		C = ((*ABSOLUTE() & 0x01) << 7);
		*ABSOLUTE() >>= 1;
		data = *ABSOLUTE() |= C;
		C >>= 7;
		PC += 3;

		break;
	}
	case _ABSOLUTE_X:
	{
		C = ((*ABSOLUTE_X() & 0x01) << 7);
		*ABSOLUTE_X() >>= 1;
		data = *ABSOLUTE_X() |= C;
		C >>= 7;
		PC += 3;

		break;
	}
	}

	// update flags

	Z = ((data == 0x00) ? 1 : 0);
	N = ((data >> 7) ? 1 : 0);
}

//Branch on Carry Clear
void _BCC()
{
	if (C == 0)
	{
		RELATIVE(PC);
	}
	PC++;
}

//Branch on Carry Set
void _BCS()
{
	if (C == 1)
	{
		RELATIVE(PC);
	}
	PC++;
}

//Branch on Result Zero
void _BEQ()
{
	if (Z == 1)
	{
		RELATIVE(PC);
	}
	PC++;
}

//Branch on Result Minus
void _BMI()
{
	if (N == 1)
	{
		RELATIVE(PC);
	}
	PC++;
}

//Branch on Result not Zero
void _BNE()
{
	if (Z == 0)
	{
		RELATIVE(PC);
	}
	PC++;
}

//Branch on Result Plus
void _BPL()
{
	if (N == 0)
	{
		RELATIVE(PC);
	}
	PC++;
}

//Branch on Overflow Clear
void _BVC()
{
	if (V == 0)
	{
		RELATIVE(PC);
	}
	PC++;
}

//Branch on Overflow Set
void _BVS()
{
	if (V == 1)
	{
		RELATIVE(PC);
	}
	PC++;
}

//Clear Carry Flag
void _CLC()
{
	C = 0x00;
	PC++;
}

//Clear Decimal Mode
void _CLD()
{
	D = 0x00;
	PC++;
}

//Clear interrupt Disable Bit
void _CLI()
{
	I = 0x00;
	PC++;
}

//Clear Overflow Flag
void _CLV()
{
	V = 0x00;
	PC++;
}

//Set Carry Flag
void _SEC()
{
	C = 0x01;
	PC++;
}

//Set Decimal Mode
void _SED()
{
	D = 0x01;
	PC++;
}

//Set Interrupt Disable Status
void _SEI()
{
	I = 0x01;
	PC++;
}

//No Operation
void _NOP()
{
	PC++;
}

//Force Break
void _BRK()
{
	PC++;
	cpu_memory[0x100 + S] = ((PC >> 8) & 0xff);
	S++;
	cpu_memory[0x100 + S] = (PC & 0xff);
	S++;
	B = 1;
	cpu_memory[0x100 + S] = S;
	S++;
	I = 1;
	PC = (cpu_memory[0xFFFE] | (cpu_memory[0xFFFF] << 8));
}

//Jump to New Location
void _JMP(uint8_t type_addressing)
{
	switch (type_addressing)
	{
		case _ABSOLUTE:
		{
			PC += *ABSOLUTE();
			break;
		}
		case _INDIRECT:
		{
			PC += *INDIRECT();
			break;
		}
	}
}

//Jump to New Location Saving Return Address
void _JSR(uint8_t type_addressing)
{
	PC--;
	cpu_memory[0x100 + S] = ((PC >> 8) & 0xff);
	S--;
	cpu_memory[0x100 + S] = (PC & 0xff);
	S--;
	PC = cpu_memory[PC + 2];
	PC <<= 8;
	PC |= cpu_memory[PC + 1];
}

//Return from Subroutine
void _RTS()
{
	S++;
	PC = cpu_memory[0x100 + S];
	S++;
	PC += (cpu_memory[0x100 + S] << 8) + 1;
}

//Return from Interrupt
void _RTI()
{
	S++;
	S = cpu_memory[0x100 + S];
	S++;
	PC = cpu_memory[0x100 + S];
	S++;
	PC |= cpu_memory[0x100 + S] << 8;
}

void nmi()
{
	cpu_memory[0x0100 + S] = (PC >> 8) & 0x00FF;
	S--;
	cpu_memory[0x0100 + S] = PC & 0x00FF;
	S--;

	B = 0;
	I = 1;

	uint16_t LO = cpu_memory[0xFFFA + 0];
	uint16_t HI = cpu_memory[0xFFFA + 1];

	PC = (HI << 8) | LO;

	clock = 8;
}

void reset()
{
	uint16_t LO = cpu_memory[0xFFFC + 0];
	uint16_t HI = cpu_memory[0xFFFC + 1];

	PC = (HI << 8) | LO;

	A = X = Y = 0;
	S = 0xFD;	

	clock = 8;
}

void irq()
{
	if (I == 0)
	{
		cpu_memory[0x100 + S] = (PC >> 8) & 0x00FF;
		S--;
		cpu_memory[0x100 + S] = PC & 0x00FF;
		S--;

		B = 0;
		I = 1;	
		
		uint16_t LO = cpu_memory[0xFFFE + 0];
		uint16_t HI = cpu_memory[0xFFFE + 1];

		PC = (HI << 8) | LO;

		clock = 7;
	}
}

int loop()
{
		if (time_execution == 0x00)
		{
			uint8_t instruction = cpu_memory[PC];

			switch (instruction)
			{
			case 0x69:
			{
				_ADC(_IMMEDIATE);
				time_execution = 0x02;
				break;
			}
			case 0x65:
			{
				_ADC(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0x75:
			{
				_ADC(_ZERO_PAGE_X);
				time_execution = 0x04;
				break;
			}
			case 0x6D:
			{
				_ADC(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0x7D:
			{
				_ADC(_ABSOLUTE_X);
				time_execution = 0x04;
				break;
			}
			case 0x79:
			{
				_ADC(_ABSOLUTE_Y);
				time_execution = 0x04;
				break;
			}
			case 0x61:
			{
				_ADC(_INDIRECT_X);
				time_execution = 0x06;
				break;
			}
			case 0x71:
			{
				_ADC(_INDIRECT_Y);
				time_execution = 0x05;
				break;
			}
			case 0x29:
			{
				_AND(_IMMEDIATE);
				time_execution = 0x02;
				break;
			}
			case 0x25:
			{
				_AND(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0x35:
			{
				_AND(_ZERO_PAGE_X);
				time_execution = 0x04;
				break;
			}
			case 0x2D:
			{
				_AND(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0x3D:
			{
				_AND(_ABSOLUTE_X);
				time_execution = 0x04;
				break;
			}
			case 0x39:
			{
				_AND(_ABSOLUTE_Y);
				time_execution = 0x04;
				break;
			}
			case 0x21:
			{
				_AND(_INDIRECT_X);
				time_execution = 0x06;
				break;
			}
			case 0x31:
			{
				_AND(_INDIRECT_Y);
				time_execution = 0x05;
				break;
			}
			case 0x0A:
			{
				_ASL(_ACCUMULATOR);
				time_execution = 0x02;
				break;
			}
			case 0x06:
			{
				_ASL(_ZERO_PAGE);
				time_execution = 0x05;
				break;
			}
			case 0x16:
			{
				_ASL(_ZERO_PAGE_X);
				time_execution = 0x06;
				break;
			}
			case 0x0E:
			{
				_ASL(_ABSOLUTE);
				time_execution = 0x06;
				break;
			}
			case 0x1E:
			{
				_ASL(_ABSOLUTE_X);
				time_execution = 0x07;
				break;
			}
			case 0x90:
			{
				_BCC();
				time_execution = 0x02;
				break;
			}
			case 0xB0:
			{
				_BCS();
				time_execution = 0x02;
				break;
			}
			case 0xF0:
			{
				_BEQ();
				time_execution = 0x02;
				break;
			}
			case 0x24:
			{
				_BIT(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0x2C:
			{
				_BIT(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0x30:
			{
				_BMI();
				time_execution = 0x02;
				break;
			}
			case 0xD0:
			{
				_BNE();
				time_execution = 0x02;
				break;
			}
			case 0x10:
			{
				_BPL();
				time_execution = 0x02;
				break;
			}
			case 0x00:
			{
				_BRK();
				time_execution = 0x07;
				break;
			}
			case 0x50:
			{
				_BVC();
				time_execution = 0x02;
				break;
			}
			case 0x70:
			{
				_BVS();
				time_execution = 0x02;
				break;
			}
			case 0x18:
			{
				_CLC();
				time_execution = 0x02;
				break;
			}
			case 0xD8:
			{
				_CLD();
				time_execution = 0x02;
				break;
			}
			case 0x58:
			{
				_CLI();
				time_execution = 0x02;
				break;
			}
			case 0xB8:
			{
				_CLV();
				time_execution = 0x02;
				break;
			}
			case 0xC9:
			{
				_CMP(_IMMEDIATE);
				time_execution = 0x02;
				break;
			}
			case 0xC5:
			{
				_CMP(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0xD5:
			{
				_CMP(_ZERO_PAGE_X);
				time_execution = 0x04;
				break;
			}
			case 0xCD:
			{
				_CMP(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0xDD:
			{
				_CMP(_ABSOLUTE_X);
				time_execution = 0x04;
				break;
			}
			case 0xD9:
			{
				_CMP(_ABSOLUTE_Y);
				time_execution = 0x04;
				break;
			}
			case 0xC1:
			{
				_CMP(_INDIRECT_X);
				time_execution = 0x06;
				break;
			}
			case 0xD1:
			{
				_CMP(_INDIRECT_Y);
				time_execution = 0x05;
				break;
			}
			case 0xE0:
			{
				_CPX(_IMMEDIATE);
				time_execution = 0x02;
				break;
			}
			case 0xE4:
			{
				_CPX(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0xEC:
			{
				_CPX(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0xC0:
			{
				_CPY(_IMMEDIATE);
				time_execution = 0x02;
				break;
			}
			case 0xC4:
			{
				_CPY(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0xCC:
			{
				_CPY(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0xC6:
			{
				_DEC(_ZERO_PAGE);
				time_execution = 0x05;
				break;
			}
			case 0xD6:
			{
				_DEC(_ZERO_PAGE_X);
				time_execution = 0x06;
				break;
			}
			case 0xCE:
			{
				_DEC(_ABSOLUTE);
				time_execution = 0x06;
				break;
			}
			case 0xDE:
			{
				_DEC(_ABSOLUTE_X);
				time_execution = 0x07;
				break;
			}
			case 0xCA:
			{
				_DEX();
				time_execution = 0x02;
				break;
			}
			case 0x88:
			{
				_DEY();
				time_execution = 0x02;
				break;
			}
			case 0x49:
			{
				_EOR(_IMMEDIATE);
				time_execution = 0x02;
				break;
			}
			case 0x45:
			{
				_EOR(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0x55:
			{
				_EOR(_ZERO_PAGE_X);
				time_execution = 0x04;
				break;
			}
			case 0x4D:
			{
				_EOR(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0x5D:
			{
				_EOR(_ABSOLUTE_X);
				time_execution = 0x04;
				break;
			}
			case 0x59:
			{
				_EOR(_ABSOLUTE_Y);
				time_execution = 0x04;
				break;
			}
			case 0x41:
			{
				_EOR(_INDIRECT_X);
				time_execution = 0x06;
				break;
			}
			case 0x51:
			{
				_EOR(_INDIRECT_Y);
				time_execution = 0x05;
				break;
			}
			case 0xE6:
			{
				_INC(_ZERO_PAGE);
				time_execution = 0x05;
				break;
			}
			case 0xF6:
			{
				_INC(_ZERO_PAGE_X);
				time_execution = 0x06;
				break;
			}
			case 0xEE:
			{
				_INC(_ABSOLUTE);
				time_execution = 0x06;
				break;
			}
			case 0xFE:
			{
				_INC(_ABSOLUTE_X);
				time_execution = 0x07;
				break;
			}
			case 0xE8:
			{
				_INX();
				time_execution = 0x02;
				break;
			}
			case 0x4C:
			{
				_JMP(_ABSOLUTE);
				time_execution = 0x03;
				break;
			}
			case 0x6C:
			{
				_JMP(_INDIRECT);
				time_execution = 0x05;
				break;
			}
			case 0x20:
			{
				_JSR(_ABSOLUTE);
				time_execution = 0x06;
				break;
			}
			case 0xA9:
			{
				_LDA(_IMMEDIATE);
				time_execution = 0x02;
				break;
			}
			case 0xA5:
			{
				_LDA(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0xB5:
			{
				_LDA(_ZERO_PAGE_X);
				time_execution = 0x04;
				break;
			}
			case 0xAD:
			{
				_LDA(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0xBD:
			{
				_LDA(_ABSOLUTE_X);
				time_execution = 0x04;
				break;
			}
			case 0xB9:
			{
				_LDA(_ABSOLUTE_Y);
				time_execution = 0x04;
				break;
			}
			case 0xA1:
			{
				_LDA(_INDIRECT_X);
				time_execution = 0x06;
				break;
			}
			case 0xB1:
			{
				_LDA(_INDIRECT_Y);
				time_execution = 0x05;
				break;
			}
			case 0xA2:
			{
				_LDX(_IMMEDIATE);
				time_execution = 0x02;
				break;
			}
			case 0xA6:
			{
				_LDX(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0xB6:
			{
				_LDX(_ZERO_PAGE_Y);
				time_execution = 0x04;
				break;
			}
			case 0xAE:
			{
				_LDX(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0xBE:
			{
				_LDX(_ABSOLUTE_Y);
				time_execution = 0x04;
				break;
			}
			case 0xA0:
			{
				_LDY(_IMMEDIATE);
				time_execution = 0x02;
				break;
			}
			case 0xA4:
			{
				_LDY(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0xB4:
			{
				_LDY(_ZERO_PAGE_Y);
				time_execution = 0x04;
				break;
			}
			case 0xAC:
			{
				_LDY(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0xBC:
			{
				_LDY(_ABSOLUTE_Y);
				time_execution = 0x04;
				break;
			}
			case 0x4A:
			{
				_LSR(_ACCUMULATOR);
				time_execution = 0x02;
				break;
			}
			case 0x46:
			{
				_LSR(_ZERO_PAGE);
				time_execution = 0x05;
				break;
			}
			case 0x56:
			{
				_LSR(_ZERO_PAGE_Y);
				time_execution = 0x06;
				break;
			}
			case 0x4E:
			{
				_LSR(_ABSOLUTE);
				time_execution = 0x06;
				break;
			}
			case 0x53:
			{
				_LSR(_ABSOLUTE_Y);
				time_execution = 0x07;
				break;
			}
			case 0x0EA:
			{
				_NOP();
				time_execution = 0x02;
				break;
			}
			case 0x09:
			{
				_ORA(_IMMEDIATE);
				time_execution = 0x02;
				break;
			}
			case 0x05:
			{
				_ORA(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0x15:
			{
				_ORA(_ZERO_PAGE_X);
				time_execution = 0x04;
				break;
			}
			case 0x0D:
			{
				_ORA(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0x1D:
			{
				_ORA(_ABSOLUTE_X);
				time_execution = 0x04;
				break;
			}
			case 0x19:
			{
				_ORA(_ABSOLUTE_Y);
				time_execution = 0x04;
				break;
			}
			case 0x01:
			{
				_ORA(_INDIRECT_X);
				time_execution = 0x06;
				break;
			}
			case 0x11:
			{
				_ORA(_INDIRECT_Y);
				time_execution = 0x05;
				break;
			}
			case 0x48:
			{
				_PHA();
				time_execution = 0x03;
				break;
			}
			case 0x08:
			{
				_PHP();
				time_execution = 0x03;
				break;
			}
			case 0x28:
			{
				_PLP();
				time_execution = 0x04;
				break;
			}
			case 0x2A:
			{
				_ROL(_ACCUMULATOR);
				time_execution = 0x02;
				break;
			}
			case 0x26:
			{
				_ROL(_ZERO_PAGE);
				time_execution = 0x05;
				break;
			}
			case 0x36:
			{
				_ROL(_ZERO_PAGE_Y);
				time_execution = 0x06;
				break;
			}
			case 0x2E:
			{
				_ROL(_ABSOLUTE);
				time_execution = 0x06;
				break;
			}
			case 0x3E:
			{
				_ROL(_ABSOLUTE_Y);
				time_execution = 0x07;
				break;
			}
			case 0x6A:
			{
				_ROR(_ACCUMULATOR);
				time_execution = 0x02;
				break;
			}
			case 0x66:
			{
				_ROR(_ZERO_PAGE);
				time_execution = 0x05;
				break;
			}
			case 0x76:
			{
				_ROR(_ZERO_PAGE_Y);
				time_execution = 0x06;
				break;
			}
			case 0x6E:
			{
				_ROR(_ABSOLUTE);
				time_execution = 0x06;
				break;
			}
			case 0x7E:
			{
				_ROR(_ABSOLUTE_Y);
				time_execution = 0x07;
				break;
			}
			case 0x40:
			{
				_RTI();
				time_execution = 0x06;
				break;
			}
			case 0x60:
			{
				_RTS();
				time_execution = 0x06;
				break;
			}
			case 0xE9:
			{
				_SBC(_IMMEDIATE);
				time_execution = 0x02;
				break;
			}
			case 0xE5:
			{
				_SBC(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0xF5:
			{
				_SBC(_ZERO_PAGE_X);
				time_execution = 0x04;
				break;
			}
			case 0xED:
			{
				_SBC(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0xFD:
			{
				_SBC(_ABSOLUTE_X);
				time_execution = 0x04;
				break;
			}
			case 0xF9:
			{
				_SBC(_ABSOLUTE_Y);
				time_execution = 0x04;
				break;
			}
			case 0xE1:
			{
				_SBC(_INDIRECT_X);
				time_execution = 0x06;
				break;
			}
			case 0xF1:
			{
				_SBC(_INDIRECT_Y);
				time_execution = 0x05;
				break;
			}
			case 0x38:
			{
				_SEC();
				time_execution = 0x02;
				break;
			}
			case 0xF8:
			{
				_SED();
				time_execution = 0x02;
				break;
			}
			case 0x78:
			{
				_SEI();
				time_execution = 0x02;
				break;
			}
			case 0x85:
			{
				_STA(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0x95:
			{
				_STA(_ZERO_PAGE_X);
				time_execution = 0x04;
				break;
			}
			case 0x8D:
			{
				_STA(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0x9D:
			{
				_STA(_ABSOLUTE_X);
				time_execution = 0x05;
				break;
			}
			case 0x99:
			{
				_STA(_ABSOLUTE_Y);
				time_execution = 0x05;
				break;
			}
			case 0x81:
			{
				_STA(_INDIRECT_X);
				time_execution = 0x06;
				break;
			}
			case 0x91:
			{
				_STA(_INDIRECT_Y);
				time_execution = 0x06;
				break;
			}
			case 0x86:
			{
				_STX(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0x96:
			{
				_STX(_ZERO_PAGE_Y);
				time_execution = 0x04;
				break;
			}
			case 0x8E:
			{
				_STX(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0x84:
			{
				_STY(_ZERO_PAGE);
				time_execution = 0x03;
				break;
			}
			case 0x94:
			{
				_STY(_ZERO_PAGE_Y);
				time_execution = 0x04;
				break;
			}
			case 0x8C:
			{
				_STY(_ABSOLUTE);
				time_execution = 0x04;
				break;
			}
			case 0xAA:
			{
				_TAX();
				time_execution = 0x02;
				break;
			}
			case 0xA8:
			{
				_TAY();
				time_execution = 0x02;
				break;
			}
			case 0xBA:
			{
				_TSX();
				time_execution = 0x02;
				break;
			}
			case 0x8A:
			{
				_TXA();
				time_execution = 0x02;
				break;
			}
			case 0x9A:
			{
				_TXS();
				time_execution = 0x02;
				break;
			}
			case 0x98:
			{
				_TYA();
				time_execution = 0x02;
				break;
			}
			
			}
		}
		else
		{
			time_execution--;
		}

	return 0;
}

