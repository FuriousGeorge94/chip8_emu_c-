#include "Chip8.cpp"
#include "Graphics.h"
#include "input.h"





int main(){
  SDL_Event event;
  Graphics graphics;
  // initialize emulator
  Chip8 chip8; ; 
  //chip8.LoadROM("./roms/opcode");
  //chip8.LoadROM("./roms/chip8-test-suite.ch8");
  //chip8.LoadROM("./roms/Tetris");
  chip8.LoadROM("./roms/Bowling");
  int prev_time = clock();
  while (1){

    int curr_time = clock();
    int change_in_time = (curr_time - prev_time);

    if(change_in_time > 0){
      //printf("%d\n", chip8.delaytimer);
      if(chip8.delaytimer > 0){
        chip8.delaytimer -= 1;
      }
      if(chip8.soundtimer > 0){
        chip8.soundtimer -= 2;
      }
      prev_time = curr_time;
      if (SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
          break;
        }
        get_keystroke(&event, &chip8);
      }
       chip8.fetch_and_decode();
       graphics.draw(chip8);
    }
   }
  

    graphics.exit();
    return 0;
  }
