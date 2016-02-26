// Samuel Weber

#ifndef DRAW_H
  #define DRAW_H

  #include "Attack.h"
  #include "Button.h"
  #include "Globals.h"
  #include "Keybind.h"
    // This probably shouldn't be here in the long run. Need to seperate the
    // keybinding logic and drawing routines (call the draws from keybind logic)
  #include "Party.h"
  #include "Render.h"


  // Draw.cpp
  void drawBattle(SDL_Renderer *ren, unique_ptr<Party> &party, TTF_Font* font,
          vector<unique_ptr<Button> > &buttons, int x, int y, int cursor,
          vector<shared_ptr<Unit> > &enemies);
  //!< Draws the battle screen.
  void drawBattleAttackText(SDL_Renderer *ren, TTF_Font *font, Attack attack,
          string attacker, string target);
  //!< Draws textbox sequence for an Attack.
  void drawBattleUpdate(SDL_Renderer *ren, unique_ptr<Party> &party,
          TTF_Font* font, vector<shared_ptr<Unit> > &enemies);
  //!< Draws changes to the battle screen as the turn progresses.
  void drawMap(unique_ptr<Terr> &terr, unique_ptr<Party> &party);
  //!< Draws the overworld map for exploration. Returns true if splining.
  void drawRebind(SDL_Renderer *ren, TTF_Font* font);
  //!< Draws the screen for rebinding keys.
  void drawSprite(SDL_Renderer *ren, shared_ptr<Sprite> sprite,
          shared_ptr<Sprite> partySprite, int i, int j);
  //!< Draws a single Sprite on the map without redrawing the map.
  void drawTitle(SDL_Renderer *ren, vector<unique_ptr<Button> > &buttons,
          int x, int y, int cursor);
  //!< Draws the title screen.

#endif