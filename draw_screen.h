#include <stdlib.h>
#include <SDL2/SDL.h>


void draw(SDL_Renderer *renderer, SDL_Window *window, int height, int width, int xpos, int ypos, Chip8 emu){
    SDL_Rect rect;
    rect.w = height;
    rect.h = width;
    for(int i = 0; i < 622; i++){
      for(int j = 0; j < 1008; j++){
        if( emu.screen[j +(i*9)] == 0 ){

            rect.x = j;
            rect.y = i;
            SDL_RenderFillRect(renderer, &rect);
            }
      }
    }

  }



