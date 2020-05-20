#include <iostream>

extern uint8_t X; // index register X
extern uint8_t Y; // index register Y

extern uint16_t PC; // address next instruction

extern uint8_t main_memory[UINT16_MAX]; // main memory bank

// addressing modes

void RELATIVE(uint16_t address)
{
	PC += (int8_t)main_memory[address + 1] + 2;
}

uint8_t* ABSOLUTE(void)
{
	uint16_t data = main_memory[PC + 2];
	data << 8;
	data |= main_memory[PC + 1];
	return &main_memory[data];
}

uint8_t* ABSOLUTE_X(void)
{
	uint16_t data = main_memory[PC + 2];
	data << 8;
	data |= main_memory[PC + 1];
	data += X;
	return &main_memory[data];
}

uint8_t* ABSOLUTE_Y(void)
{
	uint16_t data = main_memory[PC + 2];
	data << 8;
	data |= main_memory[PC + 1];
	data += Y;
	return &main_memory[data];
}

uint8_t* ZERO_PAGE_X(void)
{
	uint8_t data = main_memory[PC + 1];
	data += X;
	return &main_memory[data];
}

uint8_t* ZERO_PAGE_Y(void)
{
	uint8_t data = main_memory[PC + 1];
	data += Y;
	return &main_memory[data];
}

uint8_t* INDIRECT(void)
{
	uint16_t data = main_memory[PC + 2];
	data << 8;
	data |= main_memory[PC + 1];
	return &main_memory[data];
}

uint8_t* INDIRECT_X(void)
{
	uint16_t data = main_memory[PC + 2];
	data << 8;
	data |= main_memory[PC + 1];
	data += X;
	return &main_memory[data];
}

uint8_t* INDIRECT_Y(void)
{
	uint16_t data = main_memory[PC + 2];
	data << 8;
	data |= main_memory[PC + 1];
	data += Y;
	return &main_memory[data];
}