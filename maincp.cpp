#include "Chip8.cpp"

void draw(SDL_Renderer *renderer, SDL_Window *window, int height, int width, Chip8 emu){
    SDL_Rect rect;
    rect.w = height;
    rect.h = width;
    for(int i = 0; i < 32; i++){
      for(int j = 0; j < 64; j++){
        if(emu.screen[j +(i*65)] == 1){
        //if((j%2 == 0) && (i%2 == 1)){
            rect.x = j*16;
            rect.y = i*16;
            SDL_RenderFillRect(renderer, &rect);
            SDL_RenderPresent(renderer);
        }
      }
    }

  }

#define WINDOW_WIDTH   1024 
#define WINDOW_HEIGHT  512
int main(){

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    int i;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);

  // initialize emulator
  Chip8 chip8; ; 
  chip8.LoadROM("IBMLogo");
  while (chip8.pc <= 0x228){

    // fetch and decode instructions
    uint16_t nib1  =  chip8.memory[chip8.pc];
    uint16_t nib2  =  chip8.memory[chip8.pc+1];
    chip8.opcode = (nib1<<8) | nib2;
    uint8_t id = (chip8.opcode >> 12) ;
    
    std::bitset<16> x(chip8.opcode);
    std::bitset<8> y(id);

    //cout << x << " " << y << "\n";
    //printf("pc:%0x %0x\n", chip8.pc, chip8.opcode);
    if(id == 0){
      chip8.OP_00E0();
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
    
      draw(renderer, window,16, 16, chip8);
    }else{
      printf("unemplemented instruction: %0x\n", id);
    }
    chip8.pc +=2;

  SDL_RenderPresent(renderer);
  }

  //draw(renderer, window,16, 16, chip8);
  while (1) {
      if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
          break;
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
