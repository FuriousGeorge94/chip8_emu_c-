#include "Chip8.cpp"

void draw(SDL_Renderer *renderer, Chip8 chip8, SDL_Texture *texture){
  SDL_UpdateTexture(texture, nullptr, chip8.screen, 4*64);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

#define WINDOW_WIDTH   1024 
#define WINDOW_HEIGHT  512
int main(){

    SDL_Event event;
    SDL_Renderer *renderer{};
    SDL_Window *window{};
    SDL_Texture *texture{};

    //create window, renderer and texture
    window = SDL_CreateWindow("chip_8", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

  // initialize emulator
  Chip8 chip8; ; 
  chip8.LoadROM("./roms/chip8-test-suite.ch8");
  while (1){

    // fetch and decode instructions
    uint16_t nib1  =  chip8.memory[chip8.pc];
    uint16_t nib2  =  chip8.memory[chip8.pc+1];
    chip8.opcode = (nib1<<8) | nib2;
    uint8_t id = (chip8.opcode >> 12) ;
    
    chip8.pc +=2;
    std::bitset<16> x(chip8.opcode);
    std::bitset<8> y(id);
    //cout << x << " " << y << "\n";
    //printf("pc:%0x %0x\n", chip8.pc, chip8.opcode);
    if(id == 0){
      if(nib2 == 0xE0){
        chip8.OP_00E0();
      }else{
        chip8.OP_00EE();
      }
    }
    // jump
    else if(id == 0x1){
      chip8.OP_1nnn();
    }
    //subroutines
    else if(id == 0x2){
      chip8.OP_2nnn();
    }
    //skip equal to x and nn
    else if(id == 0x3){
      chip8.OP_3xnn();
    }
    //skip not equal to x and nn
    else if(id == 0x4){
      chip8.OP_4xnn();
    }
    // skip equal to x and y
    else if(id == 0x5){
      chip8.OP_5xy0();
    }
    // set register
    else if(id == 0x6){
      chip8.OP_6xkk();
    }
    // add
    else if(id == 0x7){
      chip8.OP_7xkk();
    }
    else if(id == 0x8){
      uint8_t last_nibb = chip8.opcode & 0x000F;
      if(last_nibb == 0x0){
        chip8.OP_8xy0();
      } 
      // binary or
      else if(last_nibb == 0x1){
        chip8.OP_8xy1();
      }
      // binary and
      else if(last_nibb == 0x2){
        chip8.OP_8xy2();
      }
      // logical xor
      else if(last_nibb == 0x3){
        chip8.OP_8xy3();
      }
      // add
      else if(last_nibb == 0x4){
        chip8.OP_8xy4();
      }
      // sub x - y
      else if(last_nibb == 0x5){
        chip8.OP_8xy5();
      }
      // shift right
      else if(last_nibb == 0x6){
        chip8.OP_8xy6();
      }
      // sub y - x
      else if(last_nibb == 0x7){
        chip8.OP_8xy7();
      }
      // shift left
      else if(last_nibb == 0xE){
        chip8.OP_8xyE();
      }
    }
    // skip if x and y not equal
    else if(id == 0x9){
      chip8.OP_9xy0();
    }
    // set index
    else if(id == 0xA){
      chip8.OP_Annn();
    }
    // random num generator
    else if(id == 0xC){
      chip8.OP_Cxnn();
    }
    // display sprite on screen
    else if(id == 0xD){
      chip8.OP_Dxyn();
      draw(renderer, chip8, texture);
    }
    // skip if key operations
    else if(id == 0xE){
      if((chip8.opcode & 0x0FF) == 0x9E){
        chip8.OP_Ex9E();
      }else{
        chip8.OP_ExA1();
      }
    }
    // timer operatons
    else if(id == 0xF){
      uint16_t end_nibb = chip8.opcode & 0x0FF;
      if(end_nibb == 0x7){
        chip8.OP_Fx07();
      }
      else if(end_nibb == 0x15){
        chip8.OP_Fx15();
      }
      else if(end_nibb == 0x18){
        chip8.OP_Fx18();
      }
      else if(end_nibb == 0x33){
        chip8.OP_Fx33();
      }
      else if(end_nibb == 0x55){
        chip8.OP_Fx55();
      }
      else if(end_nibb == 0x65){
        chip8.OP_Fx65();
      }
      // Font loading
      else{
        chip8.OP_Fx29();
      }
    }

    else{ 
      printf("unemplemented instruction: %0x\n", id);
    }
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        break;
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_DestroyTexture(texture);
  SDL_Quit();
  return 0;
}





//for (int i = 0; i < (sizeof(chip8.screen)/sizeof(uint32_t)); i++){ if(i%64 == 0){
//printf(" %d\n", i);
//}
//if(chip8.screen[i]){
//printf("#");
//}else{
//printf("-");
//}
//}
