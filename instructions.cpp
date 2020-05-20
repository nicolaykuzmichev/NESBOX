#include <iostream>
#include "types_addressing.h"

extern uint8_t A; // accumulator
extern uint8_t X; // index register X
extern uint8_t Y; // index register Y
extern uint8_t S; // stack pointer

extern bool C; // carry flag
extern bool Z; // zero flag
extern bool I; // interrupt flag
extern bool D; // BCD flag
extern bool B; // BRK flag
extern bool V; // overflow flag
extern bool N; // negative flag

extern uint16_t PC; // address next instruction

extern uint8_t main_memory[UINT16_MAX]; // main memory bank

