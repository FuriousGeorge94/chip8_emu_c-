#include <stdlib.h>

#include <SDL2/SDL.h>

#define WINDOW_WIDTH  1008 
#define WINDOW_HEIGHT  622

int main(void) {
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    int i;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    //SDL_Rect dstrect;
    SDL_Rect srcrect;

    srcrect.w = 16;
    srcrect.h = 16;

    //dstrect.x = 640/2;
    //dstrect.y = 480/2;
    //dstrect.w = 32;
    //dstrect.h = 32;


    for (i = 0; i < WINDOW_WIDTH; i+=32){
      for(int j= 0;j<WINDOW_HEIGHT;j+=32){
        srcrect.x = i;
        srcrect.y = j;
        //SDL_RenderDrawRect(renderer, &srcrect);
        if(i == 32){
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }else{
          SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        }
          SDL_RenderFillRect(renderer, &srcrect);
      }
     }

    SDL_RenderPresent(renderer);
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
