#include <iostream>

// addressing modes

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

void RELATIVE(uint16_t address);

uint8_t* ABSOLUTE(void);
uint8_t* ABSOLUTE_X(void);
uint8_t* ABSOLUTE_Y(void);

uint8_t* ZERO_PAGE_X(void);
uint8_t* ZERO_PAGE_Y(void);

uint8_t* INDIRECT(void);
uint8_t* INDIRECT_X(void);
uint8_t* INDIRECT_Y(void);