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
  bool drawMap(unique_ptr<Party> &party);
  //!< Draws the overworld map for exploration. Returns true if splining.
  void drawRebind(SDL_Renderer *ren, TTF_Font* font);
  //!< Draws the screen for rebinding keys.
  bool drawSprite(shared_ptr<Tile> tile,
          unique_ptr<Party> &party, int i, int j);
  //!< Draws a single Sprite on the map without redrawing the map.
  void drawTitle(SDL_Renderer *ren, vector<unique_ptr<Button> > &buttons);
  //!< Draws the title screen.

#endif