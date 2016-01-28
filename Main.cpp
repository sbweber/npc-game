// Samuel Weber

  #include "Draw.h"
  #include "EventLoops.h"
  #include "Keybind.h"
  #include "Globals.h"
  #include "Party.h"
  #include "Terr.h"

int main(int argc, char **argv)
{
  // Create window, do basic setup
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    quit("SDL_Init", 1);
  SDL_Window *win = SDL_CreateWindow("NPC-Game", 10, 30, SCREEN_WIDTH,
          SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (win == nullptr)
    quit("SDL_CreateWindow", 2);
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr)
    quit("SDL_CreateRenderer", 3);
  if (TTF_Init() != 0)
    quit("TTF_Init", 4);
  TTF_Font *font = TTF_OpenFont("resources/ClearSans-Light.ttf", 20);
  if (font == nullptr)
    quit("TTF_OpenFont", 5);

  // SDL vars
  SDL_Event e;
  bool quit = false;

  // backend vars
  unique_ptr<Party> party(new Party(ren));
  gameState state = TITLE;

  // spritesheets
  SDL_Texture *tiles = loadTexture("Tiles.png", ren);

  loadKeys();

  while (!quit)
    while (SDL_PollEvent(&e))
      quit = mainLoop(e, font, tiles, party, state);

  SDL_DestroyTexture(tiles);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  TTF_CloseFont(font);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  return 0;
}  // int main(int argc, char **argv)

