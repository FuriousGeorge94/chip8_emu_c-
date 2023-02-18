#include "Chip8.cpp"
#include "Graphics.h"


#define WINDOW_WIDTH   1024 
#define WINDOW_HEIGHT  512
int main(){

  SDL_Event event;
  Graphics graphics;
  // initialize emulator
  Chip8 chip8; ; 
  chip8.LoadROM("./roms/IBMLogo");
  while (1){
   chip8.fetch_and_decode();
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        break;
   graphics.draw(chip8);
  }

  graphics.exit();
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
