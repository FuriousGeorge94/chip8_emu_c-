//credit austin morlan for guide
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <bitset>
#include <stack>
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
    std::stack<uint16_t> stack; 
    uint8_t  sp{};
    uint8_t  delaytimer{};
    uint8_t  soundtimer{};
    uint8_t  keys[16]{};
    uint32_t screen[64*32]{};
    uint16_t opcode;


    Chip8();
    void fetch_and_decode();
    void LoadROM(char const* filename);
    void OP_00E0();
    void OP_00EE();
    void OP_1nnn();
    void OP_2nnn();
    void OP_3xnn();
    void OP_4xnn();
    void OP_5xy0();
    void OP_6xkk();
    void OP_7xkk();
    void OP_8xy0();
    void OP_8xy1();
    void OP_8xy2();
    void OP_8xy3();
    void OP_8xy4();
    void OP_8xy5();
    void OP_8xy6();
    void OP_8xy7();
    void OP_8xyE();
    void OP_9xy0();
    void OP_Annn();
    void OP_Cxnn();
    void OP_Dxyn();
    void OP_Ex9E();
    void OP_ExA1();
    void OP_Fx07();
    void OP_Fx15();
    void OP_Fx18();
    void OP_Fx1E();
    void OP_Fx0A();
    void OP_Fx29();
    void OP_Fx33();
    void OP_Fx55();
    void OP_Fx65();
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



// clears the screen
void Chip8::OP_00E0()
{
  memset(screen, 0, sizeof(screen));
}

// returns from subroutine by popping old pc's value of the stack
void Chip8::OP_00EE()
{
  pc = stack.top();
  stack.pop();
}

// Jump to nnn
void Chip8::OP_1nnn()
{
  uint16_t addr = opcode & 0x0FFFu;
  pc = addr;
}

// push pc to the stack before calling subroutine located at nnn  
void Chip8::OP_2nnn()
{
  uint16_t subroutine_start = opcode & 0x0FFFu;
  stack.push(pc);
  pc = subroutine_start;
}

// increments pc if register x is equal  nn
void Chip8::OP_3xnn()
{
  uint8_t  reg_x = (opcode & 0x0F00u) >> 8u;
  uint16_t value = (opcode & 0x00FFu);
  if(registers[reg_x] == value){
    pc += 2;
  }
}

// increments pc if register x does not equal to nn
void Chip8::OP_4xnn()
{
  uint8_t  reg_x = (opcode & 0x0F00u) >> 8u;
  uint16_t value = (opcode & 0x00FFu);
  if(registers[reg_x] != value){
    pc += 2;
  }
}

// increments pc if register x does not equal register y 
void Chip8::OP_5xy0()
{
  uint8_t  reg_x = (opcode & 0x0F00u) >> 8u;
  uint8_t  reg_y = (opcode & 0x00F0u);
  if(registers[reg_x] == registers[reg_y]){
    pc += 2;
  }
}

// stores the value kk into register x
void Chip8::OP_6xkk()
{
  uint16_t value       =  opcode & 0x00FFu;
  uint8_t  target_addr = (opcode & 0x0F00u)>>8u;
  registers[target_addr] = value;
}

// adds value kk with the val at register x then stores
// result in register x
void Chip8::OP_7xkk()
{
  uint16_t value          = opcode & 0x00FFu;
  uint8_t  target_addr    = (opcode & 0x0F00u)>>8;
  uint16_t reg_add_result = value + registers[target_addr];
  registers[target_addr]  = reg_add_result;
}

// val at register x is set to val at register y
void Chip8::OP_8xy0()
{
  uint8_t  reg_x = (opcode & 0x0F00u) >> 8u;
  uint8_t  reg_y = (opcode & 0x00F0u) >> 4u;
  registers[reg_x] = registers[reg_y];
}

// val at register x is set to or of itself and val at register y
void Chip8::OP_8xy1()
{
  uint8_t  reg_x = (opcode & 0x0F00u) >> 8u;
  uint8_t  reg_y = (opcode & 0x00F0u) >> 4u;
  //printf("x = %0x, y = %0x ",registers[reg_x], registers[reg_y]);
  registers[reg_x] = registers[reg_x] | registers[reg_y];
  //printf("x|y = %0x\n ",registers[reg_x]);
}

// val at register x is set to the and of itself and val at register y
void Chip8::OP_8xy2()
{
  uint8_t  reg_x = (opcode & 0x0F00u) >> 8u;
  uint8_t  reg_y = (opcode & 0x00F0u) >> 4u;
  registers[reg_x] = registers[reg_x] & registers[reg_y];
}

// val at register x is set to xor of itself and val at register y
void Chip8::OP_8xy3()
{
  uint8_t  reg_x = (opcode & 0x0F00u) >> 8u;
  uint8_t  reg_y = (opcode & 0x00F0u) >> 4u;
  registers[reg_x] = registers[reg_x] ^ registers[reg_y];
}

// val at register x is added and val at register y and stored in
// register x
void Chip8::OP_8xy4()
{
  uint8_t  reg_x = (opcode & 0x0F00u) >> 8u;
  uint8_t  reg_y = (opcode & 0x00F0u) >> 4u;
  printf("reg_x: %0x, reg_y: %0x\n", reg_x, reg_y);
  if (((int) registers[reg_x] + (int) registers[reg_y]) > 255){
    registers[0xF] = 1; 
  }
  registers[reg_x] += registers[reg_y];
}

// val at register x is subrtracted with val at register y and stored
// in register x
void Chip8::OP_8xy5()
{
  uint8_t  reg_x = (opcode & 0x0F00u) >> 8u;
  uint8_t  reg_y = (opcode & 0x00F0u) >> 4u;
  if ( registers[reg_x] > registers[reg_y] ){
    registers[0xF] = 1; 
  }else{
    registers[0xF] = 0;
  }
  registers[reg_x] -=  registers[reg_y];
}

// shifts tha val in register x by 1 to the right
void Chip8::OP_8xy6()
{
  uint8_t  reg_x = (opcode & 0x0F00u) >> 8u;
  uint8_t  reg_y = (opcode & 0x00F0u) >> 4u;
  registers[reg_x] >>= 1;
}

// val at register y is subrtracted with val at register x and stored
// in register x
void Chip8::OP_8xy7()
{
  uint8_t  reg_x = (opcode & 0x0F00u) >> 8u;
  uint8_t  reg_y = (opcode & 0x00F0u) >> 4u;

  if ( registers[reg_y] > registers[reg_x] ){
    registers[0xF] = 1; 
  }else{
    registers[0xF] = 0;
    registers[reg_x] = registers[reg_y] - registers[reg_x];
  }
}


// shifts val in register x by 1 to the left
void Chip8::OP_8xyE()
{
  uint8_t  reg_x = (opcode & 0x0F00u) >> 8u;
  uint8_t  reg_y = (opcode & 0x00F0u) >> 4u;
  registers[reg_x] <<= 1;
}

// skips instruction if val in register x doesn't equal
// val in register y
void Chip8::OP_9xy0()
{
  uint8_t  reg_x = (opcode & 0x0F00u) >> 8u;
  uint8_t  reg_y = (opcode & 0x00F0u) >> 4u;
  if(registers[reg_x] != registers[reg_y]){
    pc += 2;
  }
}

// Stores nnn in register 
void Chip8::OP_Annn()
{
  uint16_t value = opcode & 0x0FFFu;
  index = value;
}


void Chip8::OP_Cxnn()
{
  uint16_t val   = opcode & 0x00FFu;
  uint8_t  reg_x = opcode & 0x0F00u;
  uint16_t rand_num = rand() % 255u;
  registers[reg_x] = val & rand_num;
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

void Chip8::OP_Ex9E()
{
  uint8_t  reg_x = opcode & 0x0F00u;
  if(keys[registers[reg_x]]){
    pc += 2;
  }
}

void Chip8::OP_ExA1()
{
  uint8_t  reg_x = opcode & 0x0F00u;
  if(!keys[registers[reg_x]]){
    pc += 2;
  }
}

void Chip8::OP_Fx07()
{
  uint8_t reg_x = opcode & 0x0F00u;
  registers[reg_x] = delaytimer;
}

void Chip8::OP_Fx15()
{
  uint8_t reg_x = opcode & 0x0F00u;
  delaytimer = registers[reg_x];
}

void Chip8::OP_Fx18()
{
  uint8_t reg_x = opcode & 0x0F00u;
  soundtimer  = registers[reg_x];
}

// adds val in register x to Index register and stores
// result in the index register
void Chip8::OP_Fx1E()
{
  uint8_t reg_x = opcode & 0x0F00u;
  index += registers[reg_x];
}

// TODO: impliment keys
void Chip8::OP_Fx0A()
{
  uint8_t reg_x = opcode & 0x0F00u;
  printf("keys need implementing");
}


// Font loading
void Chip8::OP_Fx29()
{
  uint8_t reg_x = (opcode & 0x0F00u)>>8;
  int char_val = registers[reg_x];
  int font_adress = char_val * 5;
  index = fontset[font_adress];
}

//binary coded decimal conversion
void Chip8::OP_Fx33()
{
  uint8_t reg_x = (opcode & 0x0F00u)>>8;
  uint16_t dec_num = registers[reg_x];
  int ones_place = dec_num%10;
  memory[index+2] = ones_place;
  dec_num = dec_num/10;
  int tens_place = dec_num%10;
  memory[index+1] = tens_place;
  dec_num = dec_num/10;
  int hundreds_place = dec_num%10;
  memory[index] = hundreds_place;
}

// store registers to memory
void Chip8::OP_Fx55()
{
  uint8_t valx = (opcode & 0x0F00u)>>8;
  for(int i = 0; i <= valx; i++){
    memory[index + i] = registers[i];
  }
}
//  loads memory starting at memory[index] into
//  registers[0::x] inclusive
void Chip8::OP_Fx65()
{
  uint8_t valx = (opcode & 0x0F00u)>>8;
  for(int i = 0; i <= valx; i++){
     registers[i] = memory[index + i];
  }
}


void Chip8::fetch_and_decode(){
    // fetch and decode instructions
    uint16_t nib1  =  memory[pc];
    uint16_t nib2  =  memory[pc+1];
    opcode = (nib1<<8) | nib2;
    uint8_t id = (opcode >> 12) ;
    
    pc +=2;
    if(id == 0){
      if(nib2 == 0xE0){
        OP_00E0();
      }else{
        OP_00EE();
      }
    }
    // jump
    else if(id == 0x1){
      OP_1nnn();
    }
    //subroutines
    else if(id == 0x2){
      OP_2nnn();
    }
    //skip equal to x and nn
    else if(id == 0x3){
      OP_3xnn();
    }
    //skip not equal to x and nn
    else if(id == 0x4){
      OP_4xnn();
    }
    // skip equal to x and y
    else if(id == 0x5){
      OP_5xy0();
    }
    // set register
    else if(id == 0x6){
      OP_6xkk();
    }
    // add
    else if(id == 0x7){
      OP_7xkk();
    }
    else if(id == 0x8){
      uint8_t last_nibb = opcode & 0x000F;
      if(last_nibb == 0x0){
        OP_8xy0();
      } 
      // binary or
      else if(last_nibb == 0x1){
        OP_8xy1();
      }
      // binary and
      else if(last_nibb == 0x2){
        OP_8xy2();
      }
      // logical xor
      else if(last_nibb == 0x3){
        OP_8xy3();
      }
      // add
      else if(last_nibb == 0x4){
        OP_8xy4();
      }
      // sub x - y
      else if(last_nibb == 0x5){
        OP_8xy5();
      }
      // shift right
      else if(last_nibb == 0x6){
        OP_8xy6();
      }
      // sub y - x
      else if(last_nibb == 0x7){
        OP_8xy7();
      }
      // shift left
      else if(last_nibb == 0xE){
        OP_8xyE();
      }
    }
    // skip if x and y not equal
    else if(id == 0x9){
      OP_9xy0();
    }
    // set index
    else if(id == 0xA){
      OP_Annn();
    }
    // random num generator
    else if(id == 0xC){
      OP_Cxnn();
    }
    // display sprite on screen
    else if(id == 0xD){
      OP_Dxyn();
    }
    // skip if key operations
    else if(id == 0xE){
      if((opcode & 0x0FF) == 0x9E){
        OP_Ex9E();
      }else{
        OP_ExA1();
      }
    }
    // timer operatons
    else if(id == 0xF){
      uint16_t end_nibb = opcode & 0x0FF;
      if(end_nibb == 0x7){
        OP_Fx07();
      }
      else if(end_nibb == 0x15){
        OP_Fx15();
      }
      else if(end_nibb == 0x18){
        OP_Fx18();
      }
      else if(end_nibb == 0x33){
        OP_Fx33();
      }
      else if(end_nibb == 0x55){
        OP_Fx55();
      }
      else if(end_nibb == 0x65){
        OP_Fx65();
      }
      // Font loading
      else{
        OP_Fx29();
      }
    }

}

