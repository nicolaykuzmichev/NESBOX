#include <iostream>
#include "SDL2-2.0.9/include/SDL.h"
#include <vector>

// addresses registers ppu in cpu_memory

#define PPUCTRL 0x2000
#define PPUMASK 0x2001
#define PPUSTATUS 0x2002 
#define OAMDDR 0x2003
#define OAMDATA 0x2004
#define PPUSCROLL 0x2005
#define PPUADDR 0x2006
#define PPUDATA 0x2007
#define OAMDMA 0x4014

// color table

#define _color_00 0x757575
#define _color_01 0x271B8F
#define _color_02 0x0000AB
#define _color_03 0x47009F
#define _color_04 0x8F0077
#define _color_05 0xAB0013
#define _color_06 0xA70000
#define _color_07 0x7F0B00
#define _color_08 0x432F00
#define _color_09 0x004700
#define _color_0A 0x005100
#define _color_0B 0x003F17
#define _color_0C 0x1B3F5F
#define _color_0D 0x000000
#define _color_0E 0x000000
#define _color_0F 0x000000
#define _color_10 0xBCBCBC
#define _color_11 0x0073EF
#define _color_12 0x233BEF
#define _color_13 0x8300F3
#define _color_14 0xBF00BF
#define _color_15 0xE7005B
#define _color_16 0xDB2B00
#define _color_17 0xCB4F0F
#define _color_18 0x8B7300
#define _color_19 0x009700
#define _color_1A 0x00AB00
#define _color_1B 0x00933B
#define _color_1C 0x00838B
#define _color_1D 0x000000
#define _color_1E 0x000000
#define _color_1F 0x000000
#define _color_20 0xFFFFFF
#define _color_21 0x3FBFFF
#define _color_22 0x5F97FF
#define _color_23 0xA78BFD
#define _color_24 0xF77BFF
#define _color_25 0xFF77B7
#define _color_26 0xFF7763
#define _color_27 0xFF9B3B
#define _color_28 0xF3BF3F
#define _color_29 0x83D313
#define _color_2A 0x4FDF4B
#define _color_2B 0x58F898
#define _color_2C 0x00EBDB
#define _color_2D 0x000000
#define _color_2E 0x000000
#define _color_2F 0x000000
#define _color_30 0xFFFFFF
#define _color_31 0xABE7FF
#define _color_32 0xC7D7FF
#define _color_33 0xD7CBFF
#define _color_34 0xFFC7FF
#define _color_35 0xFFC7DB
#define _color_36 0xFFBFB3
#define _color_37 0xFFDBAB
#define _color_38 0xFFE7A3
#define _color_39 0xE3FFA3
#define _color_3A 0xABF3BF
#define _color_3B 0xB3FFCF
#define _color_3C 0x9FFFF3
#define _color_3D 0x000000
#define _color_3E 0x000000
#define _color_3F 0x000000

using namespace std;

// ppu memory

extern vector<uint8_t> ppu_memory(0x3FFF);
/*
$0000-$0FFF CHR-ROM
$1000-$1FFF CHR-ROM
$2000-$23FF VRAM 1 Symbols
$23C0-$23FF VRAM 1 Atributies
$2400-$26BF VRAM 2 Symbols
$27C0-$27FF VRAM 2 Atributies
$2800-$2BBF VRAM 3 Symbols
$2BC0-$2BFF VRAM 3 Atributies
$2C00-$2FBF VRAM 4 Symbols
$2FC0-$2FFF VRAM 4 Atributies

$3F00-$3F0F PALLETE 
$3F10-$3F1F PALLETE SPRITES
*/

extern vector<uint8_t> OAM(0x100);