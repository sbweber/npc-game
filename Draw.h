// Samuel Weber

#ifndef DRAW_H
  #define DRAW_H

  #include "Button.h"
  #include "Globals.h"
  #include "Keybind.h"
    // This probably shouldn't be here in the long run. Need to seperate the
    // keybinding logic and drawing routines (call the draws from keybind logic)
  #include "Party.h"
  #include "Render.h"


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
  bool drawUnit(SDL_Renderer *ren, Tile* tile, Party *party, int i, int j);
  //!< Draws a single unit on the map without redrawing the map.

#endif