#include <cstring> 
#include <bitset> 
#include <stdio.h> 
#include <iostream> 
#include "Chip8.cpp"
// Clear
void Chip8::OP_00E0()
{
  memset(screen, 0, sizeof(screen));
}

// Jump to nnn
void Chip8::OP_1nnn()
{
  uint16_t addr = opcode & 0x0FFFu;
  pc = addr;
}

// stores the value kk into register x
  //0101 1010 1001 1010
void Chip8::OP_6xkk()
{
  uint16_t value      =  opcode & 0x00FFu;
  uint8_t target_addr = (opcode & 0x0F00u)>>8;
  registers[target_addr] = value;
}

// adds value kk with the val at register x then stores
// result in register x
void Chip8::OP_7xkk()
{
  uint16_t value      =   opcode & 0x00FFu;
  uint8_t  target_addr = (opcode & 0x0F00u)>>8;
  uint16_t reg_add_result = value + registers[target_addr];
  registers[target_addr] = reg_add_result;
}

// Stores nnn in register I
void Chip8::OP_Annn()
{
  uint16_t value = opcode & 0x0FFFu;
  index = value;
}

// display n-byte sprite dxyn
#define SCREEN_HEIGHT = 64
#define SCREEN_WIDTH  = 32
void Chip8::OP_dxyn()
{
  uint8_t sprite_height = opcode & 0x000Fu;
  uint8_t  xcord  = (opcode & 0x0F00u)>>8u;
  uint8_t  ycord  = (opcode & 0x00F0u)>>4u;

  uint16_t mem_location = index;
  uint8_t  screen_x  = registers[xcord] & 63;
  uint8_t  screen_y  = registers[ycord] & 31;
  registers[0xf] = 0;

  for (int i = 0; i < sprite_height; i++){
    uint8_t spr_byte = memory[index];
    for (int j = 0; j < 8;j++){
      uint8_t sprite_pix = (spr_byte>>j) + 0x8u;
      uint32_t screen_pix = screen[j + 8*i];
      screen[j + (8*i)] = sprite_pix^screen_pix;
    }
   }
}

