class Graphics{
  public:



};
void draw(SDL_Renderer *renderer, Chip8 chip8, SDL_Texture *texture){
  SDL_UpdateTexture(texture, nullptr, chip8.screen, 4*64);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

