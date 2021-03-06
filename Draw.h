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
  void drawBattleInit(SDL_Renderer *ren, unique_ptr<Party> &party,
          TTF_Font* font, vector<shared_ptr<Unit> > &enemies);
  //!< Draws the initial battle screen on battle start.
  //!< NEITHER CLEARS NOR PRESENTS RENDERER! UTILITY FUNCTION!
  void drawBattleUpdate(SDL_Renderer *ren, unique_ptr<Party> &party,
          TTF_Font* font, vector<shared_ptr<Unit> > &enemies);
  //!< Draws changes to the battle screen as the turn progresses.
  //!< Currently redraws Party/Enemy HPs.
  void drawMap(unique_ptr<Terrain> &terr, unique_ptr<Party> &party);
  //!< Draws the overworld map for exploration.
  void drawRebind(SDL_Renderer *ren, TTF_Font* font);
  //!< Draws the screen for rebinding keys.
  void drawSprite(SDL_Renderer *ren, shared_ptr<Sprite> sprite,
          shared_ptr<Sprite> partySprite, int i, int j);
  //!< Draws a single Sprite on the map without redrawing the map.
  void drawTextbox(SDL_Renderer *ren, TTF_Font *font, const string &str,
          SDL_Color color = { 255, 255, 255, 255 });
  //!< Draws as many textboxes as needed to display a text string.
  void drawTitle(SDL_Renderer *ren, vector<unique_ptr<Button> > &buttons,
          int x, int y, int cursor);
  //!< Draws the title screen.

#endif