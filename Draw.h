// Samuel Weber

#ifndef DRAW_H
  #define DRAW_H

  #include "Button.h"
  #include "Keybind.h"
    // This probably shouldn't be here in the long run. Need to seperate the
    // keybinding logic and drawing routines (call the draws from keybind logic)
  #include "Globals.h"
  #include "Party.h"

  class Button;
  class Party;
  class Tile;

  // Draw.cpp
  void drawBattle(SDL_Renderer *ren, TTF_Font* font);
  //!< Draws the battle screen. Currently stubbed.
  void drawMap(SDL_Renderer *ren, SDL_Texture *tiles, Party *party);
  //!< Draws the overworld map for exploration.
  void drawRebind(SDL_Renderer *ren, TTF_Font* font);
  //!< Draws the screen for rebinding keys.
  void drawScreen(gameState &state, SDL_Renderer *ren, TTF_Font* font,
          SDL_Texture *tiles, Party *party, Button *toGame);
  //!< Chooses what kind of screen to draw based on current gamestate.
  void drawTitle(SDL_Renderer *ren, Button *toGame);
  //!< Draws the title screen.
  void drawUnit(SDL_Renderer *ren, Tile* tilePtr, int i, int j);
  //!< Draws a single unit on the map without redrawing the map.
  void getClips(SDL_Rect* clips, int numClips, int rows, int cWidth,
          int cHeight);
  //!< Fills in sprite clips from spritesheet.
  SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren);
  //!< Loads a texture
  SDL_Texture* renderText(SDL_Renderer *ren, TTF_Font *font, const string &str,
          SDL_Color color = {255, 255, 255, 255});
  //!< Renders text into a picture. Text is white by default.
  void renderTextbox(SDL_Renderer *ren, TTF_Font *font, const string &str,
          SDL_Color color = {255, 255, 255, 255});
  //!< Draws a textbox with text on the screen. Text is white by default.
  void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
          SDL_Rect *clip = nullptr, const double angle = 0,
          const SDL_RendererFlip flip = SDL_FLIP_NONE);
  //!< Renders a texture to the screen.
  void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
          SDL_Rect *clip = nullptr, const double angle = 0,
          const SDL_RendererFlip flip = SDL_FLIP_NONE);
  //!< Renders a texture to the screen.
  void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w,
          int h, const double angle = 0,
          const SDL_RendererFlip flip = SDL_FLIP_NONE);
  //!< Renders a texture to the screen.
  void tileBackground(SDL_Texture *tile, SDL_Renderer *ren);
  //!< Given a texture, regularly tiles it (as a square tile) over the screen.

#endif