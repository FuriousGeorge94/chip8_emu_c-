//credit austin morlan for guide
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>

class Chip8
{
  public:
    uint8_t  registers[16]{};
    uint8_t  memory[4096]{};
    uint16_t index{};
    uint16_t pc{};
    uint16_t stack[16]{};
    uint8_t  sp{};
    uint8_t  delaytimer{};
    uint8_t  soundtimer{};
    uint8_t  keypad[16]{};
    uint32_t screen[64*32]{};
    uint16_t opcode;

    Chip8();
    void LoadROM(char const* filename);
    void OP_00E0();
    void OP_1nnn();
    void OP_6xkk();
    void OP_7xkk();
    void OP_Annn();
    void OP_Dxyn();
};



const unsigned int START_ADDR = 0x200;


// load fontset
const unsigned int FONTSET_SIZE = 80;
uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};



const unsigned int FONTSET_START_ADDR = 0x50;


// initialize program counter
Chip8::Chip8(){
  pc = START_ADDR;
  for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
  {
    memory[FONTSET_START_ADDR+i] = fontset[i];
  }
}


// Load Rom into Memory
void Chip8::LoadROM(char const* filename)

{
  std::ifstream file(filename, std::ios::binary | std::ios::ate);

  if(file.is_open())
  {
    std::streampos size = file.tellg();
    char* buffer = new char[size];

    file.seekg(0, std::ios::beg);
    file.read(buffer, size);
    file.close();
    for (long i = 0; i < size; ++i){
      memory[START_ADDR + i] = buffer[i];
      //printf(" %0lX: %0X/n", (START_ADDR+i), memory[START_ADDR+i]);
    }
  delete[] buffer;
  }
}
//----------
// Opcodes
//----------
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
  uint16_t value          = opcode & 0x00FFu;
  uint8_t  target_addr    = (opcode & 0x0F00u)>>8;
  uint16_t reg_add_result = value + registers[target_addr];
  //printf("%0x + %0x =  %0x\n",value, registers[target_addr], reg_add_result);
  registers[target_addr]  = reg_add_result;
}

// Stores nnn in register 
void Chip8::OP_Annn()
{
  uint16_t value = opcode & 0x0FFFu;
  index = value;
}


// display n-byte sprite dxyn
#define SCREEN_HEIGHT 32
#define SCREEN_WIDTH  64
void Chip8::OP_Dxyn()
{

  uint8_t  sprite_height = opcode & 0x000Fu;
  uint8_t  x_value     = (opcode & 0x0F00u)>>8u;
  uint8_t  y_value     = (opcode & 0x00F0u)>>4u;
  uint8_t  screen_x  = registers[x_value] ;
  uint8_t  screen_y  = registers[y_value] ;

  registers[0xf] = 0;
  uint32_t start_idx = (64*screen_y) + screen_x;

  for(int i = 0; i < sprite_height;i++){
    uint32_t screen_idx = start_idx + i*64;
    uint8_t mask_byte = memory[index+i];

    for(int j = 0; j < 8; j++){
      uint8_t mask_bit = (mask_byte>>(7-j))& 1u;
      if(mask_bit){
        mask_bit = 1u;
        if(screen[screen_idx+j] == 0xFFFFFFFF){
          registers[0xf] = 1;
        }
        screen[screen_idx+j]  = screen[screen_idx+j] ^ 0xFFFFFFFF;
      }
    }
  }
 }






