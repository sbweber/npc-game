//Samuel Weber

#ifndef DRAW_H
  #define DRAW_H

  #include "Button.h"
  #include "Keybind.h" //this probably shouldn't be here in the long run. Need to seperate the keybinding logic and drawing routines (call the draws from keybind logic)
  #include "Include.h"
  #include "Terr.h"

  class Button;
  class Sprite;
  class Terr;
  class Tile;

  //Draw.cpp
  void drawBattle(SDL_Renderer *ren, TTF_Font* font);
  void drawMap(SDL_Renderer *ren, SDL_Texture *tiles, Terr *terr, Sprite *party);
  void drawRebind(SDL_Renderer *ren, TTF_Font* font);
  void drawScreen(gameState &state, SDL_Renderer *ren, TTF_Font* font, SDL_Texture *tiles, Terr *terr, Sprite *party, Button *toGame); //chooses what kind of screen to draw
  void drawTitle(SDL_Renderer *ren, Button *toGame);
  void drawUnit(SDL_Renderer *ren, Tile* tilePtr, int i, int j);
  void getClips(SDL_Rect* clips, int numClips, int rows, int cWidth, int cHeight);
  SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren);
  SDL_Texture* renderText(SDL_Renderer *ren, TTF_Font *font, const string &str, SDL_Color color = (SDL_Color){255, 255, 255, 255}); //white by default
  void renderTextbox(SDL_Renderer *ren, TTF_Font *font, const string &str, SDL_Color color = (SDL_Color){255, 255, 255, 255}); //white by default
  void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr, const double angle = 0, const SDL_RendererFlip flip = SDL_FLIP_NONE);
  void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr, const double angle = 0, const SDL_RendererFlip flip = SDL_FLIP_NONE);
  void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, const double angle = 0, const SDL_RendererFlip flip = SDL_FLIP_NONE);
  void tileBackground(SDL_Texture *tile, SDL_Renderer *ren);

#endif