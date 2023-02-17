class Graphics{

#define WINDOW_HEIGHT  512
#define WINDOW_WIDTH   1024 
  
  public:
    Graphics();
    void draw(Chip8 chip8);
    void exit();

  private:
    SDL_Renderer *renderer{};
    SDL_Window *window{};
    SDL_Texture *texture{};
};



Graphics::Graphics(){
    //create window, renderer and texture
    window = SDL_CreateWindow("chip_8", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
}

void Graphics::draw(Chip8 chip8){
  SDL_UpdateTexture(texture, nullptr, chip8.screen, 4*64);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

void Graphics::exit(){
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_DestroyTexture(texture);
  SDL_Quit();
}

