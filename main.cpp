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

    //SDL_Event event;
    //SDL_Renderer *renderer{};
    //SDL_Window *window{};
    //SDL_Texture *texture{};

    ////create window, renderer and texture
    //window = SDL_CreateWindow("chip_8", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    //renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

  // initialize emulator
  Chip8 chip8; ; 
  chip8.LoadROM("./roms/test_opcode.ch8");
  while (chip8.pc <= 0x1000){

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
    // set register
    else if(id == 0x6){
      chip8.OP_6xkk();
    }
    else if(id == 0x7){
      chip8.OP_7xkk();
    }
    else if(id == 0xA){
      chip8.OP_Annn();
    }
    else if(id == 0xD){
      chip8.OP_Dxyn();
      //draw(renderer, chip8, texture);
    }else if(id == 0x2){
      chip8.OP_2nnn();

    }else{ 
      printf("unemplemented instruction: %0x\n", id);
    }

  }

  //while (1) {
      //if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
          //break;
  //}
  //SDL_DestroyRenderer(renderer);
  //SDL_DestroyWindow(window);
  //SDL_DestroyTexture(texture);
  //SDL_Quit();
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
